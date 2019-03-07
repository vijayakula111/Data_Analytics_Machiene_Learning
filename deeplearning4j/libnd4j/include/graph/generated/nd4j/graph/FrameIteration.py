# automatically generated by the FlatBuffers compiler, do not modify

# namespace: graph

import flatbuffers

class FrameIteration(object):
    __slots__ = ['_tab']

    @classmethod
    def GetRootAsFrameIteration(cls, buf, offset):
        n = flatbuffers.encode.Get(flatbuffers.packer.uoffset, buf, offset)
        x = FrameIteration()
        x.Init(buf, n + offset)
        return x

    # FrameIteration
    def Init(self, buf, pos):
        self._tab = flatbuffers.table.Table(buf, pos)

    # FrameIteration
    def Frame(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(4))
        if o != 0:
            return self._tab.String(o + self._tab.Pos)
        return None

    # FrameIteration
    def Iteration(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(6))
        if o != 0:
            return self._tab.Get(flatbuffers.number_types.Uint16Flags, o + self._tab.Pos)
        return 0

def FrameIterationStart(builder): builder.StartObject(2)
def FrameIterationAddFrame(builder, frame): builder.PrependUOffsetTRelativeSlot(0, flatbuffers.number_types.UOffsetTFlags.py_type(frame), 0)
def FrameIterationAddIteration(builder, iteration): builder.PrependUint16Slot(1, iteration, 0)
def FrameIterationEnd(builder): return builder.EndObject()
