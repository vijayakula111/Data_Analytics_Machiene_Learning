package org.nd4j.imports.TFGraphs;

import org.apache.commons.io.FileUtils;
import org.junit.Before;
import org.junit.ClassRule;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.TemporaryFolder;
import org.nd4j.autodiff.samediff.SameDiff;
import org.nd4j.linalg.BaseNd4jTest;
import org.nd4j.linalg.api.buffer.DataType;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.factory.Nd4j;
import org.nd4j.linalg.factory.Nd4jBackend;
import org.nd4j.linalg.io.ClassPathResource;
import org.nd4j.shade.jackson.core.type.TypeReference;
import org.nd4j.shade.jackson.databind.ObjectMapper;

import java.io.File;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.util.*;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class ValidateZooModelPredictions {

    @Rule
    public TemporaryFolder testDir = new TemporaryFolder();

    @Before
    public void before() {
        Nd4j.create(1);
        Nd4j.setDataType(DataType.DOUBLE);
        Nd4j.getRandom().setSeed(123);
    }


    @Test
    public void testMobilenetV1() throws Exception {
        TFGraphTestZooModels.currentTestDir = testDir.newFolder();

        //Load model
        String path = "tf_graphs/zoo_models/mobilenet_v1_0.5_128/tf_model.txt";
        File resource = new ClassPathResource(path).getFile();
        SameDiff sd = TFGraphTestZooModels.LOADER.apply(resource, "mobilenet_v1_0.5_128");

        //Load data
        //Because we don't have DataVec NativeImageLoader in ND4J tests due to circular dependencies, we'll load the image previously saved...
        File imgFile = new ClassPathResource("deeplearning4j-zoo/goldenretriever_rgb128_unnormalized_nchw_INDArray.bin").getFile();
        INDArray img = Nd4j.readBinary(imgFile).castTo(DataType.FLOAT);
        img = img.permute(0,2,3,1).dup();   //to NHWC

        //Mobilenet V1 - not sure, but probably using inception preprocessing
        //i.e., scale to (-1,1) range
        //Image is originally 0 to 255
        img.divi(255).subi(0.5).muli(2);

        double min = img.minNumber().doubleValue();
        double max = img.maxNumber().doubleValue();

        assertTrue(min >= -1 && min <= -0.6);
        assertTrue(max <= 1 && max >= 0.6);

        //Perform inference
        List<String> inputs = sd.inputs();
        assertEquals(1, inputs.size());
        List<String> outputs = sd.outputs();
        assertEquals(1, outputs.size());

        String out = outputs.get(0);
        Map<String,INDArray> m = sd.exec(Collections.singletonMap(inputs.get(0), img), out);

        INDArray outArr = m.get(out);


        System.out.println("SHAPE: " + Arrays.toString(outArr.shape()));
        System.out.println(outArr);

        INDArray argmax = outArr.argMax(1);

        //Load labels
        List<String> labels = labels();

        int classIdx = argmax.getInt(0);
        String className = labels.get(classIdx);
        String expClass = "golden retriever";
        double prob = outArr.getDouble(classIdx);

        System.out.println("Predicted class: \"" + className + "\" - probability = " + prob);
        assertEquals(expClass, className);
    }


    @Test
    public void testResnetV2() throws Exception {
        TFGraphTestZooModels.currentTestDir = testDir.newFolder();

        //Load model
        String path = "tf_graphs/zoo_models/resnetv2_imagenet_frozen_graph/tf_model.txt";
        File resource = new ClassPathResource(path).getFile();
        SameDiff sd = TFGraphTestZooModels.LOADER.apply(resource, "resnetv2_imagenet_frozen_graph");

        //Load data
        //Because we don't have DataVec NativeImageLoader in ND4J tests due to circular dependencies, we'll load the image previously saved...
        File imgFile = new ClassPathResource("deeplearning4j-zoo/goldenretriever_rgb224_unnormalized_nchw_INDArray.bin").getFile();
        INDArray img = Nd4j.readBinary(imgFile).castTo(DataType.FLOAT);
        img = img.permute(0,2,3,1).dup();   //to NHWC

        //Resnet v2 - NO external normalization, just resize and center crop
        // https://github.com/tensorflow/models/blob/d32d957a02f5cffb745a4da0d78f8432e2c52fd4/research/tensorrt/tensorrt.py#L70
        // https://github.com/tensorflow/models/blob/1af55e018eebce03fb61bba9959a04672536107d/official/resnet/imagenet_preprocessing.py#L253-L256

        //Perform inference
        List<String> inputs = sd.inputs();
        assertEquals(1, inputs.size());

        String out = "softmax_tensor";
        Map<String,INDArray> m = sd.exec(Collections.singletonMap(inputs.get(0), img), out);

        INDArray outArr = m.get(out);


        System.out.println("SHAPE: " + Arrays.toString(outArr.shape()));
        System.out.println(outArr);

        INDArray argmax = outArr.argMax(1);

        //Load labels
        List<String> labels = labels();

        int classIdx = argmax.getInt(0);
        String className = labels.get(classIdx);
        String expClass = "golden retriever";
        double prob = outArr.getDouble(classIdx);

        System.out.println("Predicted class: " + classIdx + " - \"" + className + "\" - probability = " + prob);
        assertEquals(expClass, className);
    }


    public static List<String> labels() throws Exception {
        File labelsFile = new ClassPathResource("tf_graphs/zoo_models/labels/imagenet_labellist.txt").getFile();
        List<String> labels = FileUtils.readLines(labelsFile, StandardCharsets.UTF_8);
        return labels;
    }
}
