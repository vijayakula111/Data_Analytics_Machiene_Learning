// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace nd4j.graph
{

using global::System;
using global::FlatBuffers;

public struct FlatNode : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static FlatNode GetRootAsFlatNode(ByteBuffer _bb) { return GetRootAsFlatNode(_bb, new FlatNode()); }
  public static FlatNode GetRootAsFlatNode(ByteBuffer _bb, FlatNode obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p.bb_pos = _i; __p.bb = _bb; }
  public FlatNode __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public int Id { get { int o = __p.__offset(4); return o != 0 ? __p.bb.GetInt(o + __p.bb_pos) : (int)0; } }
  public string Name { get { int o = __p.__offset(6); return o != 0 ? __p.__string(o + __p.bb_pos) : null; } }
#if ENABLE_SPAN_T
  public Span<byte> GetNameBytes() { return __p.__vector_as_span(6); }
#else
  public ArraySegment<byte>? GetNameBytes() { return __p.__vector_as_arraysegment(6); }
#endif
  public byte[] GetNameArray() { return __p.__vector_as_array<byte>(6); }
  public OpType OpType { get { int o = __p.__offset(8); return o != 0 ? (OpType)__p.bb.GetSbyte(o + __p.bb_pos) : OpType.TRANSFORM_FLOAT; } }
  public long OpNum { get { int o = __p.__offset(10); return o != 0 ? __p.bb.GetLong(o + __p.bb_pos) : (long)0; } }
  public FlatProperties? Properties(int j) { int o = __p.__offset(12); return o != 0 ? (FlatProperties?)(new FlatProperties()).__assign(__p.__indirect(__p.__vector(o) + j * 4), __p.bb) : null; }
  public int PropertiesLength { get { int o = __p.__offset(12); return o != 0 ? __p.__vector_len(o) : 0; } }
  public int Input(int j) { int o = __p.__offset(14); return o != 0 ? __p.bb.GetInt(__p.__vector(o) + j * 4) : (int)0; }
  public int InputLength { get { int o = __p.__offset(14); return o != 0 ? __p.__vector_len(o) : 0; } }
#if ENABLE_SPAN_T
  public Span<byte> GetInputBytes() { return __p.__vector_as_span(14); }
#else
  public ArraySegment<byte>? GetInputBytes() { return __p.__vector_as_arraysegment(14); }
#endif
  public int[] GetInputArray() { return __p.__vector_as_array<int>(14); }
  public IntPair? InputPaired(int j) { int o = __p.__offset(16); return o != 0 ? (IntPair?)(new IntPair()).__assign(__p.__indirect(__p.__vector(o) + j * 4), __p.bb) : null; }
  public int InputPairedLength { get { int o = __p.__offset(16); return o != 0 ? __p.__vector_len(o) : 0; } }
  public int Output(int j) { int o = __p.__offset(18); return o != 0 ? __p.bb.GetInt(__p.__vector(o) + j * 4) : (int)0; }
  public int OutputLength { get { int o = __p.__offset(18); return o != 0 ? __p.__vector_len(o) : 0; } }
#if ENABLE_SPAN_T
  public Span<byte> GetOutputBytes() { return __p.__vector_as_span(18); }
#else
  public ArraySegment<byte>? GetOutputBytes() { return __p.__vector_as_arraysegment(18); }
#endif
  public int[] GetOutputArray() { return __p.__vector_as_array<int>(18); }
  public double ExtraParams(int j) { int o = __p.__offset(20); return o != 0 ? __p.bb.GetDouble(__p.__vector(o) + j * 8) : (double)0; }
  public int ExtraParamsLength { get { int o = __p.__offset(20); return o != 0 ? __p.__vector_len(o) : 0; } }
#if ENABLE_SPAN_T
  public Span<byte> GetExtraParamsBytes() { return __p.__vector_as_span(20); }
#else
  public ArraySegment<byte>? GetExtraParamsBytes() { return __p.__vector_as_arraysegment(20); }
#endif
  public double[] GetExtraParamsArray() { return __p.__vector_as_array<double>(20); }
  public long ExtraInteger(int j) { int o = __p.__offset(22); return o != 0 ? __p.bb.GetLong(__p.__vector(o) + j * 8) : (long)0; }
  public int ExtraIntegerLength { get { int o = __p.__offset(22); return o != 0 ? __p.__vector_len(o) : 0; } }
#if ENABLE_SPAN_T
  public Span<byte> GetExtraIntegerBytes() { return __p.__vector_as_span(22); }
#else
  public ArraySegment<byte>? GetExtraIntegerBytes() { return __p.__vector_as_arraysegment(22); }
#endif
  public long[] GetExtraIntegerArray() { return __p.__vector_as_array<long>(22); }
  public bool ExtraBools(int j) { int o = __p.__offset(24); return o != 0 ? 0!=__p.bb.Get(__p.__vector(o) + j * 1) : false; }
  public int ExtraBoolsLength { get { int o = __p.__offset(24); return o != 0 ? __p.__vector_len(o) : 0; } }
#if ENABLE_SPAN_T
  public Span<byte> GetExtraBoolsBytes() { return __p.__vector_as_span(24); }
#else
  public ArraySegment<byte>? GetExtraBoolsBytes() { return __p.__vector_as_arraysegment(24); }
#endif
  public bool[] GetExtraBoolsArray() { return __p.__vector_as_array<bool>(24); }
  public int Dimensions(int j) { int o = __p.__offset(26); return o != 0 ? __p.bb.GetInt(__p.__vector(o) + j * 4) : (int)0; }
  public int DimensionsLength { get { int o = __p.__offset(26); return o != 0 ? __p.__vector_len(o) : 0; } }
#if ENABLE_SPAN_T
  public Span<byte> GetDimensionsBytes() { return __p.__vector_as_span(26); }
#else
  public ArraySegment<byte>? GetDimensionsBytes() { return __p.__vector_as_arraysegment(26); }
#endif
  public int[] GetDimensionsArray() { return __p.__vector_as_array<int>(26); }
  public int Device { get { int o = __p.__offset(28); return o != 0 ? __p.bb.GetInt(o + __p.bb_pos) : (int)0; } }
  public int ScopeId { get { int o = __p.__offset(30); return o != 0 ? __p.bb.GetInt(o + __p.bb_pos) : (int)0; } }
  public string ScopeName { get { int o = __p.__offset(32); return o != 0 ? __p.__string(o + __p.bb_pos) : null; } }
#if ENABLE_SPAN_T
  public Span<byte> GetScopeNameBytes() { return __p.__vector_as_span(32); }
#else
  public ArraySegment<byte>? GetScopeNameBytes() { return __p.__vector_as_arraysegment(32); }
#endif
  public byte[] GetScopeNameArray() { return __p.__vector_as_array<byte>(32); }
  public string OutputNames(int j) { int o = __p.__offset(34); return o != 0 ? __p.__string(__p.__vector(o) + j * 4) : null; }
  public int OutputNamesLength { get { int o = __p.__offset(34); return o != 0 ? __p.__vector_len(o) : 0; } }
  public string OpName { get { int o = __p.__offset(36); return o != 0 ? __p.__string(o + __p.bb_pos) : null; } }
#if ENABLE_SPAN_T
  public Span<byte> GetOpNameBytes() { return __p.__vector_as_span(36); }
#else
  public ArraySegment<byte>? GetOpNameBytes() { return __p.__vector_as_arraysegment(36); }
#endif
  public byte[] GetOpNameArray() { return __p.__vector_as_array<byte>(36); }
  public DataType OutputTypes(int j) { int o = __p.__offset(38); return o != 0 ? (DataType)__p.bb.GetSbyte(__p.__vector(o) + j * 1) : (DataType)0; }
  public int OutputTypesLength { get { int o = __p.__offset(38); return o != 0 ? __p.__vector_len(o) : 0; } }
#if ENABLE_SPAN_T
  public Span<byte> GetOutputTypesBytes() { return __p.__vector_as_span(38); }
#else
  public ArraySegment<byte>? GetOutputTypesBytes() { return __p.__vector_as_arraysegment(38); }
#endif
  public DataType[] GetOutputTypesArray() { return __p.__vector_as_array<DataType>(38); }
  public FlatArray? Scalar { get { int o = __p.__offset(40); return o != 0 ? (FlatArray?)(new FlatArray()).__assign(__p.__indirect(o + __p.bb_pos), __p.bb) : null; } }

  public static Offset<FlatNode> CreateFlatNode(FlatBufferBuilder builder,
      int id = 0,
      StringOffset nameOffset = default(StringOffset),
      OpType opType = OpType.TRANSFORM_FLOAT,
      long opNum = 0,
      VectorOffset propertiesOffset = default(VectorOffset),
      VectorOffset inputOffset = default(VectorOffset),
      VectorOffset inputPairedOffset = default(VectorOffset),
      VectorOffset outputOffset = default(VectorOffset),
      VectorOffset extraParamsOffset = default(VectorOffset),
      VectorOffset extraIntegerOffset = default(VectorOffset),
      VectorOffset extraBoolsOffset = default(VectorOffset),
      VectorOffset dimensionsOffset = default(VectorOffset),
      int device = 0,
      int scope_id = 0,
      StringOffset scope_nameOffset = default(StringOffset),
      VectorOffset outputNamesOffset = default(VectorOffset),
      StringOffset opNameOffset = default(StringOffset),
      VectorOffset outputTypesOffset = default(VectorOffset),
      Offset<FlatArray> scalarOffset = default(Offset<FlatArray>)) {
    builder.StartObject(19);
    FlatNode.AddOpNum(builder, opNum);
    FlatNode.AddScalar(builder, scalarOffset);
    FlatNode.AddOutputTypes(builder, outputTypesOffset);
    FlatNode.AddOpName(builder, opNameOffset);
    FlatNode.AddOutputNames(builder, outputNamesOffset);
    FlatNode.AddScopeName(builder, scope_nameOffset);
    FlatNode.AddScopeId(builder, scope_id);
    FlatNode.AddDevice(builder, device);
    FlatNode.AddDimensions(builder, dimensionsOffset);
    FlatNode.AddExtraBools(builder, extraBoolsOffset);
    FlatNode.AddExtraInteger(builder, extraIntegerOffset);
    FlatNode.AddExtraParams(builder, extraParamsOffset);
    FlatNode.AddOutput(builder, outputOffset);
    FlatNode.AddInputPaired(builder, inputPairedOffset);
    FlatNode.AddInput(builder, inputOffset);
    FlatNode.AddProperties(builder, propertiesOffset);
    FlatNode.AddName(builder, nameOffset);
    FlatNode.AddId(builder, id);
    FlatNode.AddOpType(builder, opType);
    return FlatNode.EndFlatNode(builder);
  }

  public static void StartFlatNode(FlatBufferBuilder builder) { builder.StartObject(19); }
  public static void AddId(FlatBufferBuilder builder, int id) { builder.AddInt(0, id, 0); }
  public static void AddName(FlatBufferBuilder builder, StringOffset nameOffset) { builder.AddOffset(1, nameOffset.Value, 0); }
  public static void AddOpType(FlatBufferBuilder builder, OpType opType) { builder.AddSbyte(2, (sbyte)opType, 0); }
  public static void AddOpNum(FlatBufferBuilder builder, long opNum) { builder.AddLong(3, opNum, 0); }
  public static void AddProperties(FlatBufferBuilder builder, VectorOffset propertiesOffset) { builder.AddOffset(4, propertiesOffset.Value, 0); }
  public static VectorOffset CreatePropertiesVector(FlatBufferBuilder builder, Offset<FlatProperties>[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddOffset(data[i].Value); return builder.EndVector(); }
  public static VectorOffset CreatePropertiesVectorBlock(FlatBufferBuilder builder, Offset<FlatProperties>[] data) { builder.StartVector(4, data.Length, 4); builder.Add(data); return builder.EndVector(); }
  public static void StartPropertiesVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static void AddInput(FlatBufferBuilder builder, VectorOffset inputOffset) { builder.AddOffset(5, inputOffset.Value, 0); }
  public static VectorOffset CreateInputVector(FlatBufferBuilder builder, int[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddInt(data[i]); return builder.EndVector(); }
  public static VectorOffset CreateInputVectorBlock(FlatBufferBuilder builder, int[] data) { builder.StartVector(4, data.Length, 4); builder.Add(data); return builder.EndVector(); }
  public static void StartInputVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static void AddInputPaired(FlatBufferBuilder builder, VectorOffset inputPairedOffset) { builder.AddOffset(6, inputPairedOffset.Value, 0); }
  public static VectorOffset CreateInputPairedVector(FlatBufferBuilder builder, Offset<IntPair>[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddOffset(data[i].Value); return builder.EndVector(); }
  public static VectorOffset CreateInputPairedVectorBlock(FlatBufferBuilder builder, Offset<IntPair>[] data) { builder.StartVector(4, data.Length, 4); builder.Add(data); return builder.EndVector(); }
  public static void StartInputPairedVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static void AddOutput(FlatBufferBuilder builder, VectorOffset outputOffset) { builder.AddOffset(7, outputOffset.Value, 0); }
  public static VectorOffset CreateOutputVector(FlatBufferBuilder builder, int[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddInt(data[i]); return builder.EndVector(); }
  public static VectorOffset CreateOutputVectorBlock(FlatBufferBuilder builder, int[] data) { builder.StartVector(4, data.Length, 4); builder.Add(data); return builder.EndVector(); }
  public static void StartOutputVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static void AddExtraParams(FlatBufferBuilder builder, VectorOffset extraParamsOffset) { builder.AddOffset(8, extraParamsOffset.Value, 0); }
  public static VectorOffset CreateExtraParamsVector(FlatBufferBuilder builder, double[] data) { builder.StartVector(8, data.Length, 8); for (int i = data.Length - 1; i >= 0; i--) builder.AddDouble(data[i]); return builder.EndVector(); }
  public static VectorOffset CreateExtraParamsVectorBlock(FlatBufferBuilder builder, double[] data) { builder.StartVector(8, data.Length, 8); builder.Add(data); return builder.EndVector(); }
  public static void StartExtraParamsVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(8, numElems, 8); }
  public static void AddExtraInteger(FlatBufferBuilder builder, VectorOffset extraIntegerOffset) { builder.AddOffset(9, extraIntegerOffset.Value, 0); }
  public static VectorOffset CreateExtraIntegerVector(FlatBufferBuilder builder, long[] data) { builder.StartVector(8, data.Length, 8); for (int i = data.Length - 1; i >= 0; i--) builder.AddLong(data[i]); return builder.EndVector(); }
  public static VectorOffset CreateExtraIntegerVectorBlock(FlatBufferBuilder builder, long[] data) { builder.StartVector(8, data.Length, 8); builder.Add(data); return builder.EndVector(); }
  public static void StartExtraIntegerVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(8, numElems, 8); }
  public static void AddExtraBools(FlatBufferBuilder builder, VectorOffset extraBoolsOffset) { builder.AddOffset(10, extraBoolsOffset.Value, 0); }
  public static VectorOffset CreateExtraBoolsVector(FlatBufferBuilder builder, bool[] data) { builder.StartVector(1, data.Length, 1); for (int i = data.Length - 1; i >= 0; i--) builder.AddBool(data[i]); return builder.EndVector(); }
  public static VectorOffset CreateExtraBoolsVectorBlock(FlatBufferBuilder builder, bool[] data) { builder.StartVector(1, data.Length, 1); builder.Add(data); return builder.EndVector(); }
  public static void StartExtraBoolsVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(1, numElems, 1); }
  public static void AddDimensions(FlatBufferBuilder builder, VectorOffset dimensionsOffset) { builder.AddOffset(11, dimensionsOffset.Value, 0); }
  public static VectorOffset CreateDimensionsVector(FlatBufferBuilder builder, int[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddInt(data[i]); return builder.EndVector(); }
  public static VectorOffset CreateDimensionsVectorBlock(FlatBufferBuilder builder, int[] data) { builder.StartVector(4, data.Length, 4); builder.Add(data); return builder.EndVector(); }
  public static void StartDimensionsVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static void AddDevice(FlatBufferBuilder builder, int device) { builder.AddInt(12, device, 0); }
  public static void AddScopeId(FlatBufferBuilder builder, int scopeId) { builder.AddInt(13, scopeId, 0); }
  public static void AddScopeName(FlatBufferBuilder builder, StringOffset scopeNameOffset) { builder.AddOffset(14, scopeNameOffset.Value, 0); }
  public static void AddOutputNames(FlatBufferBuilder builder, VectorOffset outputNamesOffset) { builder.AddOffset(15, outputNamesOffset.Value, 0); }
  public static VectorOffset CreateOutputNamesVector(FlatBufferBuilder builder, StringOffset[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddOffset(data[i].Value); return builder.EndVector(); }
  public static VectorOffset CreateOutputNamesVectorBlock(FlatBufferBuilder builder, StringOffset[] data) { builder.StartVector(4, data.Length, 4); builder.Add(data); return builder.EndVector(); }
  public static void StartOutputNamesVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static void AddOpName(FlatBufferBuilder builder, StringOffset opNameOffset) { builder.AddOffset(16, opNameOffset.Value, 0); }
  public static void AddOutputTypes(FlatBufferBuilder builder, VectorOffset outputTypesOffset) { builder.AddOffset(17, outputTypesOffset.Value, 0); }
  public static VectorOffset CreateOutputTypesVector(FlatBufferBuilder builder, DataType[] data) { builder.StartVector(1, data.Length, 1); for (int i = data.Length - 1; i >= 0; i--) builder.AddSbyte((sbyte)data[i]); return builder.EndVector(); }
  public static VectorOffset CreateOutputTypesVectorBlock(FlatBufferBuilder builder, DataType[] data) { builder.StartVector(1, data.Length, 1); builder.Add(data); return builder.EndVector(); }
  public static void StartOutputTypesVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(1, numElems, 1); }
  public static void AddScalar(FlatBufferBuilder builder, Offset<FlatArray> scalarOffset) { builder.AddOffset(18, scalarOffset.Value, 0); }
  public static Offset<FlatNode> EndFlatNode(FlatBufferBuilder builder) {
    int o = builder.EndObject();
    return new Offset<FlatNode>(o);
  }
  public static void FinishFlatNodeBuffer(FlatBufferBuilder builder, Offset<FlatNode> offset) { builder.Finish(offset.Value); }
  public static void FinishSizePrefixedFlatNodeBuffer(FlatBufferBuilder builder, Offset<FlatNode> offset) { builder.FinishSizePrefixed(offset.Value); }
};


}
