from unittest import TestCase,TestSuite
from mdsdata import *
from mdsscalar import *
from mdsarray import *
from compound import *
import numpy

class dataTests(TestCase):

    def setUp(self):
        pass

    def basicBinaryOperators(self):
        def doit(operator,x,y,ans):
            z=eval('x'+operator+'y')
            check=None
            try:
                check=z==ans
            except:
                pass
            if isinstance(check,numpy.ndarray):
                self.assertEqual(check.all(),True)
            else:
                self.assertEqual(z,ans)
            self.assertEqual(type(z),type(ans))
            return

        doit('+',Uint8(30),Uint8(2),Uint8(32))
        doit('-',Uint8(30),Uint8(2),Uint8(28))
        doit('*',Uint8(30),Uint8(2),Uint8(60))
        doit('/',Uint8(30),Uint8(2),Uint8(15))
        doit('|',Uint8(4),Uint8(1),Uint8(5))
        doit('&',Uint8(255),Uint8(6),Uint8(6))
        doit('>',Uint8(4),Uint8(1),True)
        doit('>=',Uint8(4),Uint8(1),True)
        doit('<',Uint8(4),Uint8(1),False)
        doit('<=',Uint8(4),Uint8(1),False)
        doit('==',Uint8(4),Uint8(1),False)
        doit('!=',Uint8(4),Uint8(1),True)
        doit('<<',Uint8(1),Uint8(3),Uint8(8))
        doit('>>',Uint8(16),Uint8(3),Uint8(2))

        doit('+',Uint16(30),Uint16(2),Uint16(32))
        doit('-',Uint16(30),Uint16(2),Uint16(28))
        doit('*',Uint16(30),Uint16(2),Uint16(60))
        doit('/',Uint16(30),Uint16(2),Uint16(15))
        doit('|',Uint16(4),Uint16(1),Uint16(5))
        doit('&',Uint16(255),Uint16(6),Uint16(6))
        doit('>',Uint16(4),Uint16(1),True)
        doit('>=',Uint16(4),Uint16(1),True)
        doit('<',Uint16(4),Uint16(1),False)
        doit('<=',Uint16(4),Uint16(1),False)
        doit('==',Uint16(4),Uint16(1),False)
        doit('!=',Uint16(4),Uint16(1),True)
        doit('<<',Uint16(1),Uint16(3),Uint16(8))
        doit('>>',Uint16(16),Uint16(3),Uint16(2))

        doit('+',Uint32(30),Uint32(2),Uint32(32))
        doit('-',Uint32(30),Uint32(2),Uint32(28))
        doit('*',Uint32(30),Uint32(2),Uint32(60))
        doit('/',Uint32(30),Uint32(2),Uint32(15))
        doit('|',Uint32(4),Uint32(1),Uint32(5))
        doit('&',Uint32(255),Uint32(6),Uint32(6))
        doit('>',Uint32(4),Uint32(1),True)
        doit('>=',Uint32(4),Uint32(1),True)
        doit('<',Uint32(4),Uint32(1),False)
        doit('<=',Uint32(4),Uint32(1),False)
        doit('==',Uint32(4),Uint32(1),False)
        doit('!=',Uint32(4),Uint32(1),True)
        doit('<<',Uint32(1),Uint32(3),Uint32(8))
        doit('>>',Uint32(16),Uint32(3),Uint32(2))

        doit('+',Uint64(30),Uint64(2),Uint64(32))
        doit('-',Uint64(30),Uint64(2),Uint64(28))
        doit('*',Uint64(30),Uint64(2),Uint64(60))
        doit('/',Uint64(30),Uint64(2),Uint64(15))
        doit('|',Uint64(4),Uint64(1),Uint64(5))
        doit('&',Uint64(255),Uint64(6),Uint64(6))
        doit('>',Uint64(4),Uint64(1),True)
        doit('>=',Uint64(4),Uint64(1),True)
        doit('<',Uint64(4),Uint64(1),False)
        doit('<=',Uint64(4),Uint64(1),False)
        doit('==',Uint64(4),Uint64(1),False)
        doit('!=',Uint64(4),Uint64(1),True)
        doit('<<',Uint64(1),Uint64(3),Uint64(8))
        doit('>>',Uint64(16),Uint64(3),Uint64(2))

        doit('+',Int8(30),Int8(2),Int8(32))
        doit('-',Int8(30),Int8(2),Int8(28))
        doit('*',Int8(30),Int8(2),Int8(60))
        doit('/',Int8(30),Int8(2),Int8(15))
        doit('|',Int8(4),Int8(1),Uint8(5))
        doit('&',Int8(255),Int8(6),Uint8(6))
        doit('>',Int8(4),Int8(1),True)
        doit('>=',Int8(4),Int8(1),True)
        doit('<',Int8(4),Int8(1),False)
        doit('<=',Int8(4),Int8(1),False)
        doit('==',Int8(4),Int8(1),False)
        doit('!=',Int8(4),Int8(1),True)
        doit('<<',Int8(1),Int8(3),Int8(8))
        doit('>>',Int8(16),Int8(3),Int8(2))

        doit('+',Int16(30),Int16(2),Int16(32))
        doit('-',Int16(30),Int16(2),Int16(28))
        doit('*',Int16(30),Int16(2),Int16(60))
        doit('/',Int16(30),Int16(2),Int16(15))
        doit('|',Int16(4),Int16(1),Uint16(5))
        doit('&',Int16(255),Int16(6),Uint16(6))
        doit('>',Int16(4),Int16(1),True)
        doit('>=',Int16(4),Int16(1),True)
        doit('<',Int16(4),Int16(1),False)
        doit('<=',Int16(4),Int16(1),False)
        doit('==',Int16(4),Int16(1),False)
        doit('!=',Int16(4),Int16(1),True)
        doit('<<',Int16(1),Int16(3),Int16(8))
        doit('>>',Int16(16),Int16(3),Int16(2))

        doit('+',Int32(30),Int32(2),Int32(32))
        doit('-',Int32(30),Int32(2),Int32(28))
        doit('*',Int32(30),Int32(2),Int32(60))
        doit('/',Int32(30),Int32(2),Int32(15))
        doit('|',Int32(4),Int32(1),Uint32(5))
        doit('&',Int32(255),Int32(6),Uint32(6))
        doit('>',Int32(4),Int32(1),True)
        doit('>=',Int32(4),Int32(1),True)
        doit('<',Int32(4),Int32(1),False)
        doit('<=',Int32(4),Int32(1),False)
        doit('==',Int32(4),Int32(1),False)
        doit('!=',Int32(4),Int32(1),True)
        doit('<<',Int32(1),Int32(3),Int32(8))
        doit('>>',Int32(16),Int32(3),Int32(2))

        doit('+',Int64(30),Int64(2),Int64(32))
        doit('-',Int64(30),Int64(2),Int64(28))
        doit('*',Int64(30),Int64(2),Int64(60))
        doit('/',Int64(30),Int64(2),Int64(15))
        doit('|',Int64(4),Int64(1),Uint64(5))
        doit('&',Int64(255),Int64(6),Uint64(6))
        doit('>',Int64(4),Int64(1),True)
        doit('>=',Int64(4),Int64(1),True)
        doit('<',Int64(4),Int64(1),False)
        doit('<=',Int64(4),Int64(1),False)
        doit('==',Int64(4),Int64(1),False)
        doit('!=',Int64(4),Int64(1),True)
        doit('<<',Int64(1),Int64(3),Int64(8))
        doit('>>',Int64(16),Int64(3),Int64(2))

        doit('+',Float32(30),Float32(2),Float32(32))
        doit('-',Float32(30),Float32(2),Float32(28))
        doit('*',Float32(30),Float32(2),Float32(60))
        doit('/',Float32(30),Float32(2),Float32(15))
        doit('|',Float32(4),Float32(1),Uint32(5))
        doit('&',Float32(255),Float32(6),Uint32(6))
        doit('>',Float32(4),Float32(1),True)
        doit('>=',Float32(4),Float32(1),True)
        doit('<',Float32(4),Float32(1),False)
        doit('<=',Float32(4),Float32(1),False)
        doit('==',Float32(4),Float32(1),False)
        doit('!=',Float32(4),Float32(1),True)
        doit('<<',Float32(1),Float32(3),Int32(8))
        doit('>>',Float32(16),Float32(3),Int32(2))

        doit('+',Float64(30),Float64(2),Float64(32))
        doit('-',Float64(30),Float64(2),Float64(28))
        doit('*',Float64(30),Float64(2),Float64(60))
        doit('/',Float64(30),Float64(2),Float64(15))
        doit('|',Float64(4),Float64(1),Uint64(5))
        doit('&',Float64(255),Float64(6),Uint64(6))
        doit('>',Float64(4),Float64(1),True)
        doit('>=',Float64(4),Float64(1),True)
        doit('<',Float64(4),Float64(1),False)
        doit('<=',Float64(4),Float64(1),False)
        doit('==',Float64(4),Float64(1),False)
        doit('!=',Float64(4),Float64(1),True)
        doit('<<',Float64(1),Float64(3),Int64(8))
        doit('>>',Float64(16),Float64(3),Int64(2))

        doit('+',Uint8Array([29,30,31]),Uint8(2),Uint8Array([31,32,33]))
        doit('-',Uint8Array([29,30,31]),Uint8(2),Uint8Array([27,28,29]))
        doit('*',Uint8Array([29,30,31]),Uint8(2),Uint8Array([58,60,62]))
        doit('/',Uint8Array([29,30,31]),Uint8(2),Uint8Array([14,15,15]))
        doit('|',Uint8Array([2,4,8]),Uint8(1),Uint8Array([3,5,9]))
        doit('&',Uint8Array([3,7,255]),Uint8(6),Uint8Array([2,6,6]))
        doit('>',Uint8Array([2,4,8]),Uint8(4),numpy.array([False,False,True]))
        doit('>=',Uint8Array([2,4,8]),Uint8(4),numpy.array([False,True,True]))
        doit('<',Uint8Array([2,4,8]),Uint8(4),numpy.array([True,False,False]))
        doit('<=',Uint8Array([2,4,8]),Uint8(4),numpy.array([True,True,False]))
        doit('==',Uint8Array([2,4,8]),Uint8(4),numpy.array([False,True,False]))
        doit('!=',Uint8Array([2,4,8]),Uint8(4),numpy.array([True,False,True]))
        doit('<<',Uint8Array([1,2,4]),Uint8(3),Uint8Array([8,16,32]))
        doit('>>',Uint8Array([8,16,32]),Uint8(3),Uint8Array([1,2,4]))

        doit('+',Uint16Array([29,30,31]),Uint16(2),Uint16Array([31,32,33]))
        doit('-',Uint16Array([29,30,31]),Uint16(2),Uint16Array([27,28,29]))
        doit('*',Uint16Array([29,30,31]),Uint16(2),Uint16Array([58,60,62]))
        doit('/',Uint16Array([29,30,31]),Uint16(2),Uint16Array([14,15,15]))
        doit('|',Uint16Array([2,4,8]),Uint16(1),Uint16Array([3,5,9]))
        doit('&',Uint16Array([3,7,255]),Uint16(6),Uint16Array([2,6,6]))
        doit('>',Uint16Array([2,4,8]),Uint16(4),numpy.array([False,False,True]))
        doit('>=',Uint16Array([2,4,8]),Uint16(4),numpy.array([False,True,True]))
        doit('<',Uint16Array([2,4,8]),Uint16(4),numpy.array([True,False,False]))
        doit('<=',Uint16Array([2,4,8]),Uint16(4),numpy.array([True,True,False]))
        doit('==',Uint16Array([2,4,8]),Uint16(4),numpy.array([False,True,False]))
        doit('!=',Uint16Array([2,4,8]),Uint16(4),numpy.array([True,False,True]))
        doit('<<',Uint16Array([1,2,4]),Uint16(3),Uint16Array([8,16,32]))
        doit('>>',Uint16Array([8,16,32]),Uint16(3),Uint16Array([1,2,4]))

        doit('+',Uint32Array([29,30,31]),Uint32(2),Uint32Array([31,32,33]))
        doit('-',Uint32Array([29,30,31]),Uint32(2),Uint32Array([27,28,29]))
        doit('*',Uint32Array([29,30,31]),Uint32(2),Uint32Array([58,60,62]))
        doit('/',Uint32Array([29,30,31]),Uint32(2),Uint32Array([14,15,15]))
        doit('|',Uint32Array([2,4,8]),Uint32(1),Uint32Array([3,5,9]))
        doit('&',Uint32Array([3,7,255]),Uint32(6),Uint32Array([2,6,6]))
        doit('>',Uint32Array([2,4,8]),Uint32(4),numpy.array([False,False,True]))
        doit('>=',Uint32Array([2,4,8]),Uint32(4),numpy.array([False,True,True]))
        doit('<',Uint32Array([2,4,8]),Uint32(4),numpy.array([True,False,False]))
        doit('<=',Uint32Array([2,4,8]),Uint32(4),numpy.array([True,True,False]))
        doit('==',Uint32Array([2,4,8]),Uint32(4),numpy.array([False,True,False]))
        doit('!=',Uint32Array([2,4,8]),Uint32(4),numpy.array([True,False,True]))
        doit('<<',Uint32Array([1,2,4]),Uint32(3),Uint32Array([8,16,32]))
        doit('>>',Uint32Array([8,16,32]),Uint32(3),Uint32Array([1,2,4]))

        doit('+',Uint64Array([29,30,31]),Uint64(2),Uint64Array([31,32,33]))
        doit('-',Uint64Array([29,30,31]),Uint64(2),Uint64Array([27,28,29]))
        doit('*',Uint64Array([29,30,31]),Uint64(2),Uint64Array([58,60,62]))
        doit('/',Uint64Array([29,30,31]),Uint64(2),Uint64Array([14,15,15]))
        doit('|',Uint64Array([2,4,8]),Uint64(1),Uint64Array([3,5,9]))
        doit('&',Uint64Array([3,7,255]),Uint64(6),Uint64Array([2,6,6]))
        doit('>',Uint64Array([2,4,8]),Uint64(4),numpy.array([False,False,True]))
        doit('>=',Uint64Array([2,4,8]),Uint64(4),numpy.array([False,True,True]))
        doit('<',Uint64Array([2,4,8]),Uint64(4),numpy.array([True,False,False]))
        doit('<=',Uint64Array([2,4,8]),Uint64(4),numpy.array([True,True,False]))
        doit('==',Uint64Array([2,4,8]),Uint64(4),numpy.array([False,True,False]))
        doit('!=',Uint64Array([2,4,8]),Uint64(4),numpy.array([True,False,True]))
        doit('<<',Uint64Array([1,2,4]),Uint64(3),Uint64Array([8,16,32]))
        doit('>>',Uint64Array([8,16,32]),Uint64(3),Uint64Array([1,2,4]))

        doit('+',Int8Array([29,30,31]),Int8(2),Int8Array([31,32,33]))
        doit('-',Int8Array([29,30,31]),Int8(2),Int8Array([27,28,29]))
        doit('*',Int8Array([29,30,31]),Int8(2),Int8Array([58,60,62]))
        doit('/',Int8Array([29,30,31]),Int8(2),Int8Array([14,15,15]))
        doit('|',Int8Array([2,4,8]),Int8(1),Uint8Array([3,5,9]))
        doit('&',Int8Array([3,7,255]),Int8(6),Uint8Array([2,6,6]))
        doit('>',Int8Array([2,4,8]),Int8(4),numpy.array([False,False,True]))
        doit('>=',Int8Array([2,4,8]),Int8(4),numpy.array([False,True,True]))
        doit('<',Int8Array([2,4,8]),Int8(4),numpy.array([True,False,False]))
        doit('<=',Int8Array([2,4,8]),Int8(4),numpy.array([True,True,False]))
        doit('==',Int8Array([2,4,8]),Int8(4),numpy.array([False,True,False]))
        doit('!=',Int8Array([2,4,8]),Int8(4),numpy.array([True,False,True]))
        doit('<<',Int8Array([1,2,4]),Int8(3),Int8Array([8,16,32]))
        doit('>>',Int8Array([8,16,32]),Int8(3),Int8Array([1,2,4]))

        doit('+',Int16Array([29,30,31]),Int16(2),Int16Array([31,32,33]))
        doit('-',Int16Array([29,30,31]),Int16(2),Int16Array([27,28,29]))
        doit('*',Int16Array([29,30,31]),Int16(2),Int16Array([58,60,62]))
        doit('/',Int16Array([29,30,31]),Int16(2),Int16Array([14,15,15]))
        doit('|',Int16Array([2,4,8]),Int16(1),Uint16Array([3,5,9]))
        doit('&',Int16Array([3,7,255]),Int16(6),Uint16Array([2,6,6]))
        doit('>',Int16Array([2,4,8]),Int16(4),numpy.array([False,False,True]))
        doit('>=',Int16Array([2,4,8]),Int16(4),numpy.array([False,True,True]))
        doit('<',Int16Array([2,4,8]),Int16(4),numpy.array([True,False,False]))
        doit('<=',Int16Array([2,4,8]),Int16(4),numpy.array([True,True,False]))
        doit('==',Int16Array([2,4,8]),Int16(4),numpy.array([False,True,False]))
        doit('!=',Int16Array([2,4,8]),Int16(4),numpy.array([True,False,True]))
        doit('<<',Int16Array([1,2,4]),Int16(3),Int16Array([8,16,32]))
        doit('>>',Int16Array([8,16,32]),Int16(3),Int16Array([1,2,4]))

        doit('+',Int32Array([29,30,31]),Int32(2),Int32Array([31,32,33]))
        doit('-',Int32Array([29,30,31]),Int32(2),Int32Array([27,28,29]))
        doit('*',Int32Array([29,30,31]),Int32(2),Int32Array([58,60,62]))
        doit('/',Int32Array([29,30,31]),Int32(2),Int32Array([14,15,15]))
        doit('|',Int32Array([2,4,8]),Int32(1),Uint32Array([3,5,9]))
        doit('&',Int32Array([3,7,255]),Int32(6),Uint32Array([2,6,6]))
        doit('>',Int32Array([2,4,8]),Int32(4),numpy.array([False,False,True]))
        doit('>=',Int32Array([2,4,8]),Int32(4),numpy.array([False,True,True]))
        doit('<',Int32Array([2,4,8]),Int32(4),numpy.array([True,False,False]))
        doit('<=',Int32Array([2,4,8]),Int32(4),numpy.array([True,True,False]))
        doit('==',Int32Array([2,4,8]),Int32(4),numpy.array([False,True,False]))
        doit('!=',Int32Array([2,4,8]),Int32(4),numpy.array([True,False,True]))
        doit('<<',Int32Array([1,2,4]),Int32(3),Int32Array([8,16,32]))
        doit('>>',Int32Array([8,16,32]),Int32(3),Int32Array([1,2,4]))

        doit('+',Int64Array([29,30,31]),Int64(2),Int64Array([31,32,33]))
        doit('-',Int64Array([29,30,31]),Int64(2),Int64Array([27,28,29]))
        doit('*',Int64Array([29,30,31]),Int64(2),Int64Array([58,60,62]))
        doit('/',Int64Array([29,30,31]),Int64(2),Int64Array([14,15,15]))
        doit('|',Int64Array([2,4,8]),Int64(1),Uint64Array([3,5,9]))
        doit('&',Int64Array([3,7,255]),Int64(6),Uint64Array([2,6,6]))
        doit('>',Int64Array([2,4,8]),Int64(4),numpy.array([False,False,True]))
        doit('>=',Int64Array([2,4,8]),Int64(4),numpy.array([False,True,True]))
        doit('<',Int64Array([2,4,8]),Int64(4),numpy.array([True,False,False]))
        doit('<=',Int64Array([2,4,8]),Int64(4),numpy.array([True,True,False]))
        doit('==',Int64Array([2,4,8]),Int64(4),numpy.array([False,True,False]))
        doit('!=',Int64Array([2,4,8]),Int64(4),numpy.array([True,False,True]))
        doit('<<',Int64Array([1,2,4]),Int64(3),Int64Array([8,16,32]))
        doit('>>',Int64Array([8,16,32]),Int64(3),Int64Array([1,2,4]))

        doit('+',Float32Array([29,30,31]),Float32(2),Float32Array([31,32,33]))
        doit('-',Float32Array([29,30,31]),Float32(2),Float32Array([27,28,29]))
        doit('*',Float32Array([29,30,31]),Float32(2),Float32Array([58,60,62]))
        doit('/',Float32Array([29,30,31]),Float32(2),Float32Array([14.5,15,15.5]))
        doit('|',Float32Array([2,4,8]),Float32(1),Uint32Array([3,5,9]))
        doit('&',Float32Array([3,7,255]),Float32(6),Uint32Array([2,6,6]))
        doit('>',Float32Array([2,4,8]),Float32(4),numpy.array([False,False,True]))
        doit('>=',Float32Array([2,4,8]),Float32(4),numpy.array([False,True,True]))
        doit('<',Float32Array([2,4,8]),Float32(4),numpy.array([True,False,False]))
        doit('<=',Float32Array([2,4,8]),Float32(4),numpy.array([True,True,False]))
        doit('==',Float32Array([2,4,8]),Float32(4),numpy.array([False,True,False]))
        doit('!=',Float32Array([2,4,8]),Float32(4),numpy.array([True,False,True]))
        doit('<<',Float32Array([1,2,4]),Float32(3),Int32Array([8,16,32]))
        doit('>>',Float32Array([8,16,32]),Float32(3),Int32Array([1,2,4]))

        doit('+',Float64Array([29,30,31]),Float64(2),Float64Array([31,32,33]))
        doit('-',Float64Array([29,30,31]),Float64(2),Float64Array([27,28,29]))
        doit('*',Float64Array([29,30,31]),Float64(2),Float64Array([58,60,62]))
        doit('/',Float64Array([29,30,31]),Float64(2),Float64Array([14.5,15,15.5]))
        doit('|',Float64Array([2,4,8]),Float64(1),Uint64Array([3,5,9]))
        doit('&',Float64Array([3,7,255]),Float64(6),Uint64Array([2,6,6]))
        doit('>',Float64Array([2,4,8]),Float64(4),numpy.array([False,False,True]))
        doit('>=',Float64Array([2,4,8]),Float64(4),numpy.array([False,True,True]))
        doit('<',Float64Array([2,4,8]),Float64(4),numpy.array([True,False,False]))
        doit('<=',Float64Array([2,4,8]),Float64(4),numpy.array([True,True,False]))
        doit('==',Float64Array([2,4,8]),Float64(4),numpy.array([False,True,False]))
        doit('!=',Float64Array([2,4,8]),Float64(4),numpy.array([True,False,True]))
        doit('<<',Float64Array([1,2,4]),Float64(3),Int64Array([8,16,32]))
        doit('>>',Float64Array([8,16,32]),Float64(3),Int64Array([1,2,4]))

        doit('+',Int64Array([29,30,31]),Int64Array([2,3,4]),Int64Array([31,33,35]))
        doit('-',Int64Array([29,30,31]),Int64Array([2,3,4]),Int64Array([27,27,27]))
        doit('*',Int64Array([29,30,31]),Int64Array([2,3,4]),Int64Array([58,90,124]))
        doit('/',Int64Array([29,30,31]),Int64Array([2,3,4]),Int64Array([14,10,7]))
        doit('|',Int64Array([2,4,8]),Int64Array([1,2,3]),Uint64Array([3,6,11]))
        doit('&',Int64Array([3,7,255]),Int64Array([6,4,7]),Uint64Array([2,4,7]))
        doit('>',Int64Array([2,4,8]),Int64Array([3,4,5]),numpy.array([False,False,True]))
        doit('>=',Int64Array([2,4,8]),Int64Array([3,4,5]),numpy.array([False,True,True]))
        doit('<',Int64Array([2,4,8]),Int64Array([3,4,5]),numpy.array([True,False,False]))
        doit('<=',Int64Array([2,4,8]),Int64Array([3,4,5]),numpy.array([True,True,False]))
        doit('==',Int64Array([2,4,8]),Int64Array([3,4,5]),numpy.array([False,True,False]))
        doit('!=',Int64Array([2,4,8]),Int64Array([3,4,5]),numpy.array([True,False,True]))
        doit('<<',Int64Array([1,2,4]),Int64Array([3,4,5]),Int64Array([8,32,128]))
        doit('>>',Int64Array([8,16,32]),Int64Array([3,2,1]),Int64Array([1,4,16]))

        doit('+',Signal(Int64Array([29,30,31]),None,None),Int64Array([2,3,4]),Signal(Int64Array([31,33,35]),None,None))
        doit('-',Signal(Int64Array([29,30,31]),None,None),Int64Array([2,3,4]),Signal(Int64Array([27,27,27]),None,None))
        doit('*',Signal(Int64Array([29,30,31]),None,None),Int64Array([2,3,4]),Signal(Int64Array([58,90,124]),None,None))
        doit('/',Signal(Int64Array([29,30,31]),None,None),Int64Array([2,3,4]),Signal(Int64Array([14,10,7]),None,None))
        doit('|',Signal(Int64Array([2,4,8]),None,None),Int64Array([1,2,3]),Signal(Uint64Array([3,6,11]),None,None))
        doit('&',Signal(Int64Array([3,7,255]),None,None),Int64Array([6,4,7]),Signal(Uint64Array([2,4,7]),None,None))
        doit('>',Signal(Int64Array([2,4,8]),None,None),Int64Array([3,4,5]),numpy.array([False,False,True]))
        doit('>=',Signal(Int64Array([2,4,8]),None,None),Int64Array([3,4,5]),numpy.array([False,True,True]))
        doit('<',Signal(Int64Array([2,4,8]),None,None),Int64Array([3,4,5]),numpy.array([True,False,False]))
        doit('<=',Signal(Int64Array([2,4,8]),None,None),Int64Array([3,4,5]),numpy.array([True,True,False]))
        doit('==',Signal(Int64Array([2,4,8]),None,None),Int64Array([3,4,5]),numpy.array([False,True,False]))
        doit('!=',Signal(Int64Array([2,4,8]),None,None),Int64Array([3,4,5]),numpy.array([True,False,True]))
        doit('<<',Signal(Int64Array([1,2,4]),None,None),Int64Array([3,4,5]),Signal(Int64Array([8,32,128]),None,None))
        doit('>>',Signal(Int64Array([8,16,32]),None,None),Int64Array([3,2,1]),Signal(Int64Array([1,4,16]),None,None))

        return

    def mathFunctions(self):
        self.assertEqual(abs(Uint8(255)),Uint8(255))
        self.assertEqual(abs(Uint16(255)),Uint16(255))
        self.assertEqual(abs(Uint32(255)),Uint32(255))
        self.assertEqual(abs(Uint64(255)),Uint64(255))
        self.assertEqual(abs(Int8(-10)),Int8(10))
        self.assertEqual(abs(Int16(-10)),Int16(10))
        self.assertEqual(abs(Int32(-10)),Int32(10))
        self.assertEqual(abs(Int64(-10)),Int64(10))
        self.assertEqual(abs(Float32(-10)),Float32(10))
        self.assertEqual(abs(Float64(-10)),Float64(10))
        self.assertEqual((abs(Uint8Array([254,255]))==Uint8Array([254,255])).all(),True)
        self.assertEqual((abs(Uint16Array([254,255]))==Uint16Array([254,255])).all(),True)
        self.assertEqual((abs(Uint32Array([254,255]))==Uint32Array([254,255])).all(),True)
        self.assertEqual((abs(Uint64Array([254,255]))==Uint64Array([254,255])).all(),True)
        self.assertEqual((abs(Int8Array([-10,-20]))==Int8Array([10,20])).all(),True)
        self.assertEqual((abs(Int16Array([-10,-20]))==Int16Array([10,20])).all(),True)
        self.assertEqual((abs(Int32Array([-10,-20]))==Int32Array([10,20])).all(),True)
        self.assertEqual((abs(Int64Array([-10,-20]))==Int64Array([10,20])).all(),True)
        self.assertEqual((abs(Float32Array([-10,-20]))==Float32Array([10,20])).all(),True)
        self.assertEqual((abs(Float64Array([-10,-20]))==Float64Array([10,20])).all(),True)
        self.assertEqual(float(Uint8(255)),255.)
        self.assertEqual(float(Uint16(255)),255.)
        self.assertEqual(float(Uint32(255)),255.)
        self.assertEqual(float(Uint64(255)),255.)
        self.assertEqual(float(Int8(127)),127.)
        self.assertEqual(float(Int16(255)),255.)
        self.assertEqual(float(Int32(255)),255.)
        self.assertEqual(float(Int64(255)),255.)
        self.assertEqual(float(Float32(255)),255.)
        self.assertEqual(float(Float64(255)),255.)
        self.assertEqual(int(Uint8(255)),255)
        self.assertEqual(int(Uint16(255)),255)
        self.assertEqual(int(Uint32(255)),255)
        self.assertEqual(int(Uint64(255)),255)
        self.assertEqual(int(Int8(127)),127)
        self.assertEqual(int(Int16(255)),255)
        self.assertEqual(int(Int32(255)),255)
        self.assertEqual(int(Int64(255)),255)
        self.assertEqual(int(Float32(255)),255)
        self.assertEqual(int(Float64(255)),255)

    def runTest(self):
        self.basicBinaryOperators()
        self.mathFunctions()
        

def suite():
    tests = ['basicBinaryOperators','mathFunctions']
    return TestSuite(map(dataTests,tests))

def test():
    """Exercise data operations"""
    print abs(makeData(-10))
    x=makeDataArray([29,30,31])
    y=makeDataArray([2,4,8])
    print x+y
    print x-y
    print x&y
    print x==y
    print x>y
    print x>=y
    print x<y
    print x<=y
    print x%y
    print x!=y
    print -x
    print x!=0
    print x|y
    print +x
    print x*y
    print x/y
    print x//y
    print x<<y
    print x>>y
    print abs(makeData(-10))
    print float(x)
    print int(x)
    print x.getDouble()
    print x.decompile()
    print x
    print x.evaluate()
    print x.getByte()
    print x.getInt()
    print x.getShape()
    print x.getByteArray()
    print x.getShortArray()
    print x.getIntArray()
    print x.getLongArray()
    print x.getString()
    print x.getUnits()
    x.setUnits('amps')
    print x.getUnits()
    x.setError(42)
    print x.getError()
    x.setHelp('This is help')
    print x.getHelp()
    x=Data.execute('data(1:100.)')
    print x+y
    print x-y
    print x&y
    print x==y
    print x>y
    print x>=y
    print x<y
    print x<=y
    print x%y
    print x!=y
    print -x
    print x!=0
    print x|y
    print +x
    print x*y
    print x/y
    print x//y
    print x<<y
    print x>>y
    print abs(makeData(-10))
    print float(x)
    print x.decompile()
    print x
    print x.evaluate()
    print x.getShape()
    print x.getByteArray()
    print x.getShortArray()
    print x.getIntArray()
    print x.getLongArray()
    print x.getString()
    print x.getUnits()
    x.setUnits('amps')
    print x.getUnits()
    x.setError(42)
    print x.getError()
    x.setHelp('This is help')
    print x.getHelp()

