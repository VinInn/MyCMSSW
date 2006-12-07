# for the time being only dataformat
#
import string, os, sys
#
class BuildFile :
    def __init__(self, filen) :
        # print filen
        self.filen = filen
        self.use=[]
        self.reallyUsed = []
        f = open(filen)
        lines = string.join(f.readlines())
        j=0
        while (1):
            i = lines.find("use",j)
            if (i<0) : break
            i = lines.find("name",i)
            if (i<0) : break
            i = lines.find("=",i)
            if (i<0) : break
            j = lines.find('>',i)
            if (j<i) : break
            # print lines[i:j+1]
            self.use.append(lines[i+1:j].strip())
        f.close()
        
    def depend(self,pack):
        self.reallyUsed.append(pack)
        return pack in self.use            
    def report(self) :
        report = []
        for pk in self.use :
            if (pk.find('/')>0 and pk not in self.reallyUsed) : report.append(pk)
        return report
    
class Headers :
    def __init__(self, filen, lpk) :
        self.localpack =lpk
        self.packages=[]
        f = open(filen)
        lines = string.join(f.readlines())
        k=0
        while (1):
            i = lines.find("#include",k)
            if (i<0) : break
            k = lines.find('\n',i)
            if (k<0) : break
            j = lines.rfind('/interface',i,k)
            if (j<i) : continue
            i = lines.rfind('"',i,j)
            if (i<0) : break
            # print lines[i:j+1]
            pk = lines[i+1:j].strip()
            if (pk!=self.localpack) :
                self.packages.append(pk)

 
class CheckDep :
     def __init__(self, src) :
         self.src=src
         self.dirs = ["DataFormats","SimDataFormats"]
         self.report = ''
         
     def check(self) :
         for bdir in self.dirs:
             topdir = self.src+'/'+bdir
             # print topdir
             for dir in os.listdir(topdir):
                 self.report += self.iterate(bdir+'/'+dir)

     def iterate(self,pack) :
         report=''
         buildfile = pack+'/BuildFile'
         try:
             bfile =BuildFile(self.src+'/'+buildfile)
         except:
             report += pack+" does not have a BuildFile\n"
         else:
             dirs = []
             dirs.append(self.src+'/'+pack+'/interface')
             dirs.append(self.src+'/'+pack+'/src')
             for dir in dirs :
                 try:
                     #print dir
                     os.stat(dir)
                 except:
                     print "NO????", dir
                     continue
                 else:
                     for fname in os.listdir(dir):
                         # print fname
                         # for each file check if include match use
                         if (fname.find(".h")<1 and fname.find(".cc")<1 ): continue
                         headers = Headers(dir+'/'+fname,pack)
                         for pk in headers.packages :
                             if (not bfile.depend(pk)) :
                                 report += pack + ' includes headers from ' + pk \
                                           + ' but does not declare it in BuildFile: '
                                 report += "Please add <use name="+pk+"> to " + pack+"/BuildFile\n"
             # and viceversa check if any use is not included"
             for pk in bfile.report() : 
                 report+= pack + ' use ' + pk + " but does not include any of its header files: "
                 report+= "Please remove <use name="+pk+"> from " +  pack+"/BuildFile\n"
 
         return report
              

def main():
    args = sys.argv[1:]
    if len(args) != 1:
        print 'usage: python checkdep.py $CMSSW_BASE/src'
        sys.exit(-1)
    c = CheckDep(args[0])
    c.check()
    print c.report
    
if __name__ == '__main__':
    main()
