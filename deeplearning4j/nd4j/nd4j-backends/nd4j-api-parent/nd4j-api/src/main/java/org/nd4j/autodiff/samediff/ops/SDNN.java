package org.nd4j.autodiff.samediff.ops;

import org.nd4j.autodiff.samediff.SDVariable;
import org.nd4j.autodiff.samediff.SameDiff;

/**
 * SameDiff general neural network operations<br>
 * Accessible via {@link SameDiff#math()}<br>
 * See also {@link SDCNN} (accessible via {@link SameDiff#cnn()} for convolutional neural network ops.<br>
 * See also {@link SDRNN} (accessible via {@link SameDiff#rnn()} for recurrent neural network ops.<br>
 *
 * @author Alex Black
 */
public class SDNN extends SDOps {
    public SDNN(SameDiff sameDiff) {
        super(sameDiff);
    }

    /**
     * Batch norm operation.
     *
     * @see #batchNorm(String, SDVariable, SDVariable, SDVariable, SDVariable, SDVariable, double, int...)
     */
    public SDVariable batchNorm(SDVariable input, SDVariable mean,
                                SDVariable variance, SDVariable gamma,
                                SDVariable beta, double epsilon, int... axis) {
        return batchNorm(null, input, mean, variance, gamma, beta, true, true, epsilon, axis);
    }

    /**
     * Batch normalization with optional application of gamma/beta args.
     * See {@link #batchNorm(String, SDVariable, SDVariable, SDVariable, SDVariable, SDVariable, double, int...)}
     */
    public SDVariable batchNorm(String name, SDVariable input, SDVariable mean,
                                SDVariable variance, SDVariable gamma,
                                SDVariable beta, boolean applyGamma, boolean applyBeta, double epsilon, int... axis) {
        SDVariable res = f().batchNorm(input, mean, variance, gamma, beta, applyGamma, applyBeta, epsilon, axis);
        return updateVariableNameAndReference(res, name);
    }

    /**
     * Neural network batch normalization operation.<br>
     * For details, see <a href="http://arxiv.org/abs/1502.03167">http://arxiv.org/abs/1502.03167</a>
     *
     * @param name     Name of the output variable
     * @param input    Input variable.
     * @param mean     Mean value. For 1d axis, this should match input.size(axis)
     * @param variance Variance value. For 1d axis, this should match input.size(axis)
     * @param gamma    Gamma value. For 1d axis, this should match input.size(axis)
     * @param beta     Beta value. For 1d axis, this should match input.size(axis)
     * @param epsilon  Epsilon constant for numerical stability (to avoid division by 0)
     * @param axis     For 2d CNN activations: 1 for NCHW format activations, or 3 for NHWC format activations.<br>
     *                 For 3d CNN activations: 1 for NCDHW format, 4 for NDHWC<br>
     *                 For 1d/RNN activations: 1 for NCW format, 2 for NWC
     * @return Output variable for batch normalization
     */
    public SDVariable batchNorm(String name, SDVariable input, SDVariable mean,
                                SDVariable variance, SDVariable gamma,
                                SDVariable beta, double epsilon, int... axis) {
        return batchNorm(name, input, mean, variance, gamma, beta, true, true, epsilon, axis);
    }

    /**
     * @see #biasAdd(String, SDVariable, SDVariable)
     */
    public SDVariable biasAdd(SDVariable input, SDVariable bias) {
        return biasAdd(null, input, bias);
    }

    /**
     * Bias addition operation: a special case of addition, typically used with CNN 4D activations and a 1D bias vector
     *
     * @param name  Name of the output variable
     * @param input 4d input variable
     * @param bias  1d bias
     * @return Output variable
     */
    public SDVariable biasAdd(String name, SDVariable input, SDVariable bias) {
        SDVariable ret = f().biasAdd(input, bias);
        return updateVariableNameAndReference(ret, name);
    }

    /**
     * @param input                  Input
     * @param inputRetainProbability Probability of retaining an input (set to 0 with probability 1-p)
     * @return
     */
    public SDVariable dropout(SDVariable input, double inputRetainProbability) {
        return dropout(null, input, inputRetainProbability);
    }

    /**
     * @param input                  Input
     * @param inputRetainProbability Probability of retaining an input (set to 0 with probability 1-p)
     * @return
     */
    public SDVariable dropout(String name, SDVariable input, double inputRetainProbability) {
        SDVariable res = f().dropout(input, inputRetainProbability);
        return updateVariableNameAndReference(res, name);
    }

    /**
     * Element-wise exponential linear unit (ELU) function:<br>
     * out = x if x > 0<br>
     * out = a * (exp(x) - 1) if x <= 0<br>
     * with constant a = 1.0
     * <p>
     * See: <a href="http://arxiv.org/abs/1511.07289">http://arxiv.org/abs/1511.07289</a>
     *
     * @param x Input variable
     * @return Output variable
     */
    public SDVariable elu(SDVariable x) {
        return elu(null, x);
    }

    /**
     * Element-wise exponential linear unit (ELU) function:<br>
     * out = x if x > 0<br>
     * out = a * (exp(x) - 1) if x <= 0<br>
     * with constant a = 1.0
     * <p>
     * See: <a href="http://arxiv.org/abs/1511.07289">http://arxiv.org/abs/1511.07289</a>
     *
     * @param name Output variable name
     * @param x    Input variable
     * @return Output variable
     */
    public SDVariable elu(String name, SDVariable x) {
        SDVariable result = f().elu(x);
        return updateVariableNameAndReference(result, name);
    }

    /**
     * Element-wise derivative exponential linear unit (ELU) function, dOut/dIn given input.
     * {@link #elu(SDVariable)}
     *
     * @param x Input variable
     * @return Output variable
     */
    public SDVariable eluDerivative(SDVariable x) {
        return eluDerivative(null, x);
    }

    /**
     * Element-wise derivative exponential linear unit (ELU) function, dOut/dIn given input.
     * {@link #elu(SDVariable)}
     *
     * @param name Output variable name
     * @param x    Input variable
     * @return Output variable
     */
    public SDVariable eluDerivative(String name, SDVariable x) {
        SDVariable result = f().eluDerivative(x);
        return updateVariableNameAndReference(result, name);
    }

    /**
     * GELU activation function - Gaussian Error Linear Units<br>
     * For more details, see <i>Gaussian Error Linear Units (GELUs)</i> - <a href="https://arxiv.org/abs/1606.08415">https://arxiv.org/abs/1606.08415</a>
     * This method uses the sigmoid approximation
     *
     * @param x Input
     * @return Output variable - GELU applied to the input
     */
    public SDVariable gelu(SDVariable x) {
        return gelu(null, x);
    }

    /**
     * GELU activation function - Gaussian Error Linear Units<br>
     * For more details, see <i>Gaussian Error Linear Units (GELUs)</i> - <a href="https://arxiv.org/abs/1606.08415">https://arxiv.org/abs/1606.08415</a>
     * This method uses the sigmoid approximation
     *
     * @param name Name of the output variable. May be null.
     * @param x    Input
     * @return Output variable - GELU applied to the input
     */
    public SDVariable gelu(String name, SDVariable x) {
        SDVariable ret = f().gelu(x, false);    //Defaults to si
        return updateVariableNameAndReference(ret, name);
    }

    /**
     * Element-wise hard sigmoid function:<br>
     * out[i] = 0 if in[i] <= -2.5<br>
     * out[1] = 0.2*in[i]+0.5 if -2.5 < in[i] < 2.5<br>
     * out[i] = 1 if in[i] >= 2.5<br>
     *
     * @param in Input variable
     * @return Output variable
     */
    public SDVariable hardSigmoid(SDVariable in) {
        return hardSigmoid(null, in);
    }

    /**
     * Element-wise hard sigmoid function:<br>
     * out[i] = 0 if in[i] <= -2.5<br>
     * out[1] = 0.2*in[i]+0.5 if -2.5 < in[i] < 2.5<br>
     * out[i] = 1 if in[i] >= 2.5<br>
     *
     * @param name Name of the output variable
     * @param in   Input variable
     * @return Output variable
     */
    public SDVariable hardSigmoid(String name, SDVariable in) {
        SDVariable ret = f().hardSigmoid(in);
        return updateVariableNameAndReference(ret, name);
    }

    /**
     * Element-wise hard tanh function:<br>
     * out[i] = -1 if in[i] <= -1<br>
     * out[1] = in[i] if -1 < in[i] < 1<br>
     * out[i] = 1 if in[i] >= 1<br>
     *
     * @param in Input variable
     * @return Output variable
     */
    public SDVariable hardTanh(SDVariable in) {
        return hardTanh(null, in);
    }

    /**
     * Element-wise hard tanh function:<br>
     * out[i] = -1 if in[i] <= -1<br>
     * out[1] = in[i] if -1 < in[i] < 1<br>
     * out[i] = 1 if in[i] >= 1<br>
     *
     * @param name Output variable name
     * @param in   Input variable
     * @return Output variable
     */
    public SDVariable hardTanh(String name, SDVariable in) {
        SDVariable result = f().hardTanh(in);
        return updateVariableNameAndReference(result, name);
    }

    /**
     * Derivative (dOut/dIn) of the element-wise hard Tanh function - {@link #hardTanh(SDVariable)}
     *
     * @param x Input
     * @return Output variable
     */
    public SDVariable hardTanhDerivative(SDVariable x) {
        return hardTanhDerivative(null, x);
    }

    /**
     * Derivative (dOut/dIn) of the element-wise hard Tanh function - {@link #hardTanh(SDVariable)}
     *
     * @param name Output variable name
     * @param x    Input
     * @return Output variable
     */
    public SDVariable hardTanhDerivative(String name, SDVariable x) {
        SDVariable result = f().hardTanhDerivative(x);
        return updateVariableNameAndReference(result, name);
    }

    /**
     * Element-wise leaky ReLU function:<br>
     * out = x if x >= 0.0<br>
     * out = alpha * x if x < cutoff<br>
     * Alpha value is most commonly set to 0.01
     *
     * @param x     Input variable
     * @param alpha Cutoff - usually 0.0
     * @return Output variable
     */
    public SDVariable leakyRelu(SDVariable x, double alpha) {
        return leakyRelu(null, x, alpha);
    }

    /**
     * Element-wise leaky ReLU function:<br>
     * out = x if x >= 0.0<br>
     * out = alpha * x if x < cutoff<br>
     * Alpha value is most commonly set to 0.01
     *
     * @param x     Input variable
     * @param alpha Cutoff - usually 0.0
     * @return Output variable
     */
    public SDVariable leakyRelu(String name, SDVariable x, double alpha) {
        SDVariable result = f().leakyRelu(x, alpha);
        return updateVariableNameAndReference(result, name);
    }

    /**
     * Leaky ReLU derivative: dOut/dIn given input.<br>
     * See {@link #leakyRelu(String, SDVariable, double)}
     *
     * @param x     Input variable
     * @param alpha Alpha value
     * @return Output variable
     */
    public SDVariable leakyReluDerivative(String name, SDVariable x, double alpha) {
        SDVariable result = f().leakyReluDerivative(x, alpha);
        return updateVariableNameAndReference(result, name);
    }

    /**
     * @see #linear(String, SDVariable, SDVariable, SDVariable)
     */
    public SDVariable linear(SDVariable input, SDVariable weights, SDVariable bias) {
        return linear(null, input, weights, bias);
    }

    /**
     * Linear layer operation: out = mmul(in,w) + bias<br>
     * Note that bias array is optional
     *
     * @param name    Name of the output variable
     * @param input   Input data
     * @param weights Weights variable
     * @param bias    Optional bias variable (may be null)
     * @return Output variable
     */
    public SDVariable linear(String name, SDVariable input, SDVariable weights, SDVariable bias) {
        SDVariable res = f().xwPlusB(input, weights, bias);
        return updateVariableNameAndReference(res, name);
    }

    /**
     * Element-wise sigmoid function: out[i] = log(sigmoid(in[i]))
     *
     * @param x Input Variable
     * @return Output variable
     */
    public SDVariable logSigmoid(SDVariable x) {
        return logSigmoid(null, x);
    }

    /**
     * Element-wise sigmoid function: out[i] = log(sigmoid(in[i]))
     *
     * @param name Name of the output variable
     * @param x    Input Variable
     * @return Output variable
     */
    public SDVariable logSigmoid(String name, SDVariable x) {
        SDVariable ret = f().logSigmoid(x);
        return updateVariableNameAndReference(ret, name);
    }

    /**
     * Log softmax activation
     *
     * @param x Input variable
     * @return Output variable
     */
    public SDVariable logSoftmax(SDVariable x) {
        return logSoftmax(null, x);
    }

    /**
     * Log softmax activation
     *
     * @param name Variable name
     * @param x    Input variable
     * @return Output variable
     */
    public SDVariable logSoftmax(String name, SDVariable x) {
        SDVariable ret = f().logSoftmax(x);
        return updateVariableNameAndReference(ret, name);
    }

    /**
     * Element-wise rectified linear function with specified cutoff:<br>
     * out[i] = in[i] if in[i] >= cutoff
     * out[i] = 0 otherwise
     *
     * @param x      Input variable
     * @param cutoff Cutoff value. Usually 0
     * @return Output variable
     */
    public SDVariable relu(SDVariable x, double cutoff) {
        return relu(null, x, cutoff);
    }

    /**
     * Element-wise rectified linear function with specified cutoff:<br>
     * out[i] = in[i] if in[i] >= cutoff
     * out[i] = 0 otherwise
     *
     * @param name   Output variable name
     * @param x      Input variable
     * @param cutoff Cutoff value. Usually 0
     * @return Output variable
     */
    public SDVariable relu(String name, SDVariable x, double cutoff) {
        SDVariable result = f().relu(x, cutoff);
        return updateVariableNameAndReference(result, name);
    }

    /**
     * Element-wise "rectified linear 6" function with specified cutoff:<br>
     * out[i] = min(max(in, cutoff), 6)
     *
     * @param x      Input variable
     * @param cutoff Cutoff value. Usually 0
     * @return Output variable
     */
    public SDVariable relu6(SDVariable x, double cutoff) {
        return relu6(null, x, cutoff);
    }

    /**
     * Element-wise "rectified linear 6" function with specified cutoff:<br>
     * out[i] = min(max(in, cutoff), 6)
     *
     * @param name   Output variable name
     * @param x      Input variable
     * @param cutoff Cutoff value. Usually 0
     * @return Output variable
     */
    public SDVariable relu6(String name, SDVariable x, double cutoff) {
        SDVariable result = f().relu6(x, cutoff);
        return updateVariableNameAndReference(result, name);
    }

    /**
     * @see #reluLayer(String, SDVariable, SDVariable, SDVariable)
     */
    public SDVariable reluLayer(SDVariable input, SDVariable weights, SDVariable bias) {
        return reluLayer(null, input, weights, bias);
    }

    /**
     * ReLU (Rectified Linear Unit) layer operation: out = relu(mmul(in,w) + bias)<br>
     * Note that bias array is optional
     *
     * @param name    Name of the output variable
     * @param input   Input data
     * @param weights Weights variable
     * @param bias    Optional bias variable (may be null)
     * @return Output variable
     */
    public SDVariable reluLayer(String name, SDVariable input, SDVariable weights, SDVariable bias) {
        SDVariable res = f().reluLayer(input, weights, bias);
        return updateVariableNameAndReference(res, name);
    }

    /**
     * Element-wise SeLU function - Scaled exponential Lineal Unit: see <a href="https://arxiv.org/abs/1706.02515">Self-Normalizing Neural Networks</a>
     * <br>
     * out[i] = scale * alpha * (exp(in[i])-1) if in[i]>0, or 0 if in[i] <= 0<br>
     * Uses default lcale and alpha values.
     *
     * @param x Input variable
     * @return Output variable
     */
    public SDVariable selu(SDVariable x) {
        return selu(null, x);
    }

    /**
     * Element-wise SeLU function - Scaled exponential Lineal Unit: see <a href="https://arxiv.org/abs/1706.02515">Self-Normalizing Neural Networks</a>
     * <br>
     * out[i] = scale * alpha * (exp(in[i])-1) if in[i]>0, or 0 if in[i] <= 0<br>
     * Uses default lcale and alpha values.
     *
     * @param name Name of the output variable
     * @param x    Input variable
     * @return Output variable
     */
    public SDVariable selu(String name, SDVariable x) {
        SDVariable ret = f().selu(x);
        return updateVariableNameAndReference(ret, name);
    }

    /**
     * Element-wise sigmoid function: out[i] = 1.0/(1+exp(-in[i]))
     *
     * @param x Input Variable
     * @return Output variable
     */
    public SDVariable sigmoid(SDVariable x) {
        return sigmoid(null, x);
    }

    /**
     * Element-wise sigmoid function: out[i] = 1.0/(1+exp(-in[i]))
     *
     * @param name Output variable name
     * @param x    Input Variable
     * @return Output variable
     */
    public SDVariable sigmoid(String name, SDVariable x) {
        SDVariable result = f().sigmoid(x);
        return updateVariableNameAndReference(result, name);
    }

    /**
     * Element-wise sigmoid function derivative: dL/dIn given input and dL/dOut
     *
     * @param x   Input Variable
     * @param wrt Gradient at the output - dL/dOut. Must have same shape as the input
     * @return Output variable
     */
    public SDVariable sigmoidDerivative(SDVariable x, SDVariable wrt) {
        return sigmoidDerivative(null, x, wrt);
    }

    /**
     * Element-wise sigmoid function derivative: dL/dIn given input and dL/dOut
     *
     * @param name Output variable name
     * @param x    Input Variable
     * @param wrt  Gradient at the output - dL/dOut. Must have same shape as the input
     * @return Output variable
     */
    public SDVariable sigmoidDerivative(String name, SDVariable x, SDVariable wrt) {
        SDVariable result = f().sigmoidDerivative(x, wrt);
        return updateVariableNameAndReference(result, name);
    }

    /**
     * Softmax activation
     *
     * @param x Input variable
     * @return Output variable
     */
    public SDVariable softmax(SDVariable x) {
        return softmax(null, x);
    }

    /**
     * Softmax activation
     *
     * @param x Input variable
     * @return Output variable
     */
    public SDVariable softmax(String name, SDVariable x) {
        SDVariable result = f().softmax(x);
        return updateVariableNameAndReference(result, name);
    }

    /**
     * @param x
     * @return
     */
    public SDVariable softmaxDerivative(String name, SDVariable x, SDVariable wrt) {
        return softmaxDerivative(name, x, wrt, null);
    }

    public SDVariable softmaxDerivative(String name, SDVariable x, SDVariable wrt, Integer dimension) {
        SDVariable result = f().softmaxDerivative(x, wrt, dimension);
        return updateVariableNameAndReference(result, name);
    }

    /**
     * Element-wise softplus function: out = log(exp(x) + 1)
     *
     * @param x Input variable
     * @return Output variable
     */
    public SDVariable softplus(SDVariable x) {
        return softplus(null, x);
    }

    /**
     * Element-wise softplus function: out = log(exp(x) + 1)
     *
     * @param name Output variable name
     * @param x    Input variable
     * @return Output variable
     */
    public SDVariable softplus(String name, SDVariable x) {
        SDVariable result = f().softplus(x);
        return updateVariableNameAndReference(result, name);
    }

    /**
     * Element-wise softsign function: out = x / (abs(x) + 1)
     *
     * @param x Input variable
     * @return Output variable
     */
    public SDVariable softsign(SDVariable x) {
        return softsign(null, x);
    }

    /**
     * Element-wise softsign function: out = x / (abs(x) + 1)
     *
     * @param name Output variable name
     * @param x    Input variable
     * @return Output variable
     */
    public SDVariable softsign(String name, SDVariable x) {
        SDVariable result = f().softsign(x);
        return updateVariableNameAndReference(result, name);
    }

    /**
     * Element-wise derivative (dOut/dIn) of the softsign function {@link #softsign(SDVariable)}
     *
     * @param x Input variable
     * @return Output varible
     */
    public SDVariable softsignDerivative(SDVariable x) {
        return softsignDerivative(null, x);
    }

    /**
     * Element-wise derivative (dOut/dIn) of the softsign function {@link #softsign(SDVariable)}
     *
     * @param name Output variable name
     * @param x    Input variable
     * @return Output varible
     */
    public SDVariable softsignDerivative(String name, SDVariable x) {
        SDVariable result = f().softsignDerivative(x);
        return updateVariableNameAndReference(result, name);
    }

    /**
     * Element-wise "swish" function: out = x * sigmoid(b*x) with b=1.0<br>
     * See: <a href="https://arxiv.org/abs/1710.05941">https://arxiv.org/abs/1710.05941</a>
     *
     * @param x Input variable
     * @return Output variable
     */
    public SDVariable swish(SDVariable x) {
        return swish(null, x);
    }

    /**
     * Element-wise "swish" function: out = x * sigmoid(b*x) with b=1.0<br>
     * See: <a href="https://arxiv.org/abs/1710.05941">https://arxiv.org/abs/1710.05941</a>
     *
     * @param name Name of the output variable
     * @param x    Input variable
     * @return Output variable
     */
    public SDVariable swish(String name, SDVariable x) {
        SDVariable ret = f().swish(x);
        return updateVariableNameAndReference(ret, name);
    }

    public SDVariable tanh(String name, SDVariable x) {
        return sd.math().tanh(name, x);
    }

    public SDVariable tanh(SDVariable x) {
        return sd.math().tanh(x);
    }
}
