// automatically generated by the FlatBuffers compiler, do not modify

package org.nd4j.graph;

import java.nio.*;
import java.lang.*;
import java.nio.ByteOrder;
import java.util.*;
import com.google.flatbuffers.*;

@SuppressWarnings("unused")
public final class FlatResponse extends Table {
  public static FlatResponse getRootAsFlatResponse(ByteBuffer _bb) { return getRootAsFlatResponse(_bb, new FlatResponse()); }
  public static FlatResponse getRootAsFlatResponse(ByteBuffer _bb, FlatResponse obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; }
  public FlatResponse __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public int status() { int o = __offset(4); return o != 0 ? bb.getInt(o + bb_pos) : 0; }

  public static int createFlatResponse(FlatBufferBuilder builder,
      int status) {
    builder.startObject(1);
    FlatResponse.addStatus(builder, status);
    return FlatResponse.endFlatResponse(builder);
  }

  public static void startFlatResponse(FlatBufferBuilder builder) { builder.startObject(1); }
  public static void addStatus(FlatBufferBuilder builder, int status) { builder.addInt(0, status, 0); }
  public static int endFlatResponse(FlatBufferBuilder builder) {
    int o = builder.endObject();
    return o;
  }
}

