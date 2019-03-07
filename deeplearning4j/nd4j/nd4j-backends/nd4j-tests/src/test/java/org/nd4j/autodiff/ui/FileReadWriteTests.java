package org.nd4j.autodiff.ui;

import com.google.flatbuffers.Table;
import lombok.extern.slf4j.Slf4j;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.TemporaryFolder;
import org.nd4j.autodiff.samediff.SDVariable;
import org.nd4j.autodiff.samediff.SameDiff;
import org.nd4j.autodiff.samediff.VariableType;
import org.nd4j.autodiff.samediff.internal.SameDiffOp;
import org.nd4j.autodiff.samediff.internal.Variable;
import org.nd4j.autodiff.samediff.serde.FlatBuffersMapper;
import org.nd4j.graph.*;
import org.nd4j.graph.ui.LogFileWriter;
import org.nd4j.linalg.api.buffer.DataType;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.factory.Nd4j;
import org.nd4j.linalg.primitives.Pair;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

@Slf4j
public class FileReadWriteTests {

    @Rule
    public TemporaryFolder testDir = new TemporaryFolder();

    @Before
    public void before() {
        Nd4j.create(1);
        Nd4j.setDefaultDataTypes(DataType.DOUBLE, DataType.DOUBLE);
        Nd4j.getRandom().setSeed(123);
    }

    @Test
    public void testSimple() throws IOException {
        SameDiff sd = SameDiff.create();
        SDVariable v = sd.var("variable", DataType.DOUBLE, 3, 4);
        SDVariable sum = v.sum();

        File f = testDir.newFile();
        if (f.exists())
            f.delete();
        System.out.println(f.getAbsolutePath());


        LogFileWriter w = new LogFileWriter(f);
        long bytesWritten = w.writeGraphStructure(sd);
        long bytesWritten2 = w.writeFinishStaticMarker();

        assertTrue(bytesWritten > 0);
        assertTrue(bytesWritten2 > 0);

        LogFileWriter.StaticInfo read = w.readStatic();
        assertEquals(2, read.getData().size());

        long fileLength = f.length();
        assertEquals(fileLength, read.getFileOffset());

        //Check graph structure:
            //Inputs
        UIGraphStructure s = (UIGraphStructure) read.getData().get(0).getSecond();
        List<String> l = new ArrayList<>(s.inputsLength());
        for (int i = 0; i < s.inputsLength(); i++) {
            l.add(s.inputs(i));
        }
        assertEquals(sd.inputs(), l);

            //Outputs
        List<String> outputs = new ArrayList<>(s.outputsLength());
        for (int i = 0; i < s.outputsLength(); i++) {
            outputs.add(s.outputs(i));
        }
        assertEquals(sd.outputs(), outputs);

            //Check variables
        int numVars = s.variablesLength();
        List<UIVariable> varsList = new ArrayList<>(numVars);
        Map<String,UIVariable> varsMap = new HashMap<>();
        for( int i=0; i<numVars; i++ ){
            UIVariable uivar = s.variables(i);
            varsList.add(uivar);
            String name = uivar.name();
            varsMap.put(name, uivar);
        }

        Map<String,Variable> sdVarsMap = sd.getVariables();
        assertEquals(sdVarsMap.keySet(), varsMap.keySet());
        for(String vName : sdVarsMap.keySet()){
            VariableType vt = sdVarsMap.get(vName).getVariable().getVariableType();
            VariableType vt2 = FlatBuffersMapper.fromVarType(varsMap.get(vName).type());
            assertEquals(vt, vt2);

            //TODO check inputs to, output of, etc
        }

        //Check ops
        int numOps = s.opsLength();
        List<UIOp> opsList = new ArrayList<>(numVars);
        Map<String,UIOp> opMap = new HashMap<>();
        for( int i=0; i<numOps; i++ ){
            UIOp uiop = s.ops(i);
            opsList.add(uiop);
            String name = uiop.name();
            opMap.put(name, uiop);
        }

        Map<String,SameDiffOp> sdOpsMap = sd.getOps();
        assertEquals(sdOpsMap.keySet(), opMap.keySet());
        //TODO check inputs, outputs etc

        assertEquals(UIInfoType.START_EVENTS, read.getData().get(1).getFirst().infoType());


        //Append a number of events
        w.registerEventName("accuracy");
        for( int iter=0; iter<3; iter++) {
            long t = System.currentTimeMillis();
            w.writeScalarEvent("accuracy", t, iter, 0, 0.5 + 0.1 * iter);
        }

        //Read events back in...
        List<Pair<UIEvent,Table>> events = w.readEvents();
        assertEquals(4, events.size());             //add name + 3 scalars

        UIAddName addName = (UIAddName) events.get(0).getRight();
        assertEquals("accuracy", addName.name());

        for( int i=1; i<4; i++ ){
            FlatArray fa = (FlatArray) events.get(i).getRight();
            INDArray arr = Nd4j.createFromFlatArray(fa);

            INDArray exp = Nd4j.scalar(0.5 + (i-1) * 0.1);
            assertEquals(exp, arr);
        }
    }
}
