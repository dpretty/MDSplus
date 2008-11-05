from ctypes import CDLL,pointer,c_void_p
import os

if os.name=='nt':
    TdiShr=CDLL('tdishr')
else:
    TdiShr=CDLL('libTdiShr.so')
    
class TdiException(Exception):
    pass

def restoreContext():
    from MDSobjects.tree import Tree
    if hasattr(Tree,'_activeTree'):
        Tree._activeTree.restoreContext()
        
def TdiExecute(expression,args=None):
    """Compile and execute a TDI expression. Format: TdiExecute('expression-string')"""
    from MDSobjects._descriptor import descriptor_xd,descriptor,MdsGetMsg
    from MDSobjects.tree import Tree
    xd=descriptor_xd()
    if args is None:
        try:
            Tree.lock()
            restoreContext()
            status=TdiShr.TdiExecute(pointer(descriptor(expression)),pointer(xd),c_void_p(-1))
        finally:
            Tree.unlock()
    else:
        if isinstance(args,tuple):
            __execute=TdiShr.TdiExecute
            exp='__execute(pointer(descriptor(expression))'
            for i in range(len(args)):
                exp=exp+',pointer(descriptor(args[%d]).toXd())' % i
            exp=exp+',pointer(xd),c_void_p(-1))'
            try:
                Tree.lock()
                restoreContext()
                status=eval(exp)
            finally:
                Tree.unlock()
        else:
            raise TypeError,'Arguments must be passed as a tuple'
    if (status & 1 != 0):
        return xd.value
    else:
        raise TdiException,MdsGetMsg(status,"Error executing expression")

def TdiCompile(expression,args=None):
    """Compile and execute a TDI expression. Format: TdiExecute('expression-string')"""
    from MDSobjects._descriptor import descriptor_xd,descriptor,MdsGetMsg
    from MDSobjects.tree import Tree
    __execute=TdiShr.TdiCompile
    xd=descriptor_xd()
    done=False
    try:
        Tree.lock()
        restoreContext()
        if args is None:
            status=__execute(pointer(descriptor(expression)),pointer(xd),c_void_p(-1))
        else:
            if isinstance(args,tuple):
                if len(args) > 0:
                    if isinstance(args[0],tuple):
                        ans = TdiCompile(expression,args[0])
                        done=True
                if not done:
                    exp='__execute(pointer(descriptor(expression))'
                    for i in range(len(args)):
                        exp=exp+',pointer(descriptor(args[%d]).toXd())' % i
                    exp=exp+',pointer(xd),c_void_p(-1))'
                    status=eval(exp)
            else:
                raise TypeError,'Arguments must be passed as a tuple'
    finally:
        Tree.unlock()
    if done:
        return ans
    if (status & 1 != 0):
            return xd.value
    else:
        raise TdiException,MdsGetMsg(status,"Error compiling expression")

def TdiDecompile(value):
    """Compile and execute a TDI expression. Format: TdiExecute('expression-string')"""
    from MDSobjects._descriptor import descriptor_xd,descriptor,MdsGetMsg
    from MDSobjects.tree import Tree
    xd=descriptor_xd()
    try:
        Tree.lock()
        restoreContext()
        status=TdiShr.TdiDecompile(pointer(descriptor(value)),pointer(xd),c_void_p(-1))
    finally:
        Tree.unlock()
    if (status & 1 != 0):
        return str(xd.value)
    else:
        raise TdiException,MdsGetMsg(status,"Error decompiling value")

def TdiEvaluate(value):
    """Compile and execute a TDI expression. Format: TdiExecute('expression-string')"""
    from MDSobjects._descriptor import descriptor_xd,descriptor,MdsGetMsg
    from MDSobjects.tree import Tree
    xd=descriptor_xd()
    try:
        Tree.lock()
        restoreContext()
        status=TdiShr.TdiEvaluate(pointer(descriptor(value).toXd()),pointer(xd),c_void_p(-1))
    finally:
        Tree.unlock()
    if (status & 1 != 0):
        return xd.value
    else:
        raise TdiException,MdsGetMsg(status,"Error evaluating value")

def TdiData(value):
    """Return primiitive data type. Format: TdiData(value)"""
    from MDSobjects._descriptor import descriptor_xd,descriptor,MdsGetMsg
    from MDSobjects.tree import Tree
    xd=descriptor_xd()
    try:
        Tree.lock()
        restoreContext()
        status=TdiShr.TdiData(pointer(descriptor(value).toXd()),pointer(xd),c_void_p(-1))
    finally:
        Tree.unlock()
    if (status & 1 != 0):
        return xd.value
    else:
        raise TdiException,MdsGetMsg(status,"Error converting value to data")

def tditest():
    """Execute Tdi expressions and display results. Format: tditest()"""
    try:
        exp=raw_input('Enter an expression: ')
        while exp:
            try:
                ans = TdiExecute(exp)
                print type(ans)
                print ans
            except Exception,e:
                print "Error executing expression: ",e
            exp=raw_input('Enter an expression: ')
    except EOFError:
        exp=False
        print ""

CvtConvertFloat=TdiShr.CvtConvertFloat
