from binascii import hexlify
from pprint   import pprint
from io       import StringIO
from sys      import argv
from os       import path



PROGRAM = argv[0]

if len(argv) < 2:
    print("No File was Specified.")
    print(f"USAGE: { PROGRAM } <PNG_FILE_PATH>.")
    exit(1)

FILE    = argv[1]

if not path.exists(FILE):
    print(f"{ FILE } No Such File.")
    exit(1)


PNG_HEADER = bytearray([137, 80, 78, 71, 13, 10, 26, 10])
PNG_HEADER_LEN = len(PNG_HEADER)

class InvalidImage(Exception): pass

class Bounds:
    def __init__(self):
       self.Left, self.Right = (0, 0)

def int_from_bytes(bytes_: bytearray | bytes) -> int: return int.from_bytes(bytes_, "big", signed=False)

class Chunk:
    def __init__(self) -> None:
        self.length = None
        self.type   = None
        self.data   = None
        self.CRC    = None
    
    def stat(self):
        return f"""
-----------------------------------------------------------

        Name:   { self.type }
        Length: { self.length }

-----------------------------------------------------------
"""  

    def Scan(self, bytes_: bytes, is_first: bool) -> bytes:
        if is_first:
            Header = bytes_[0:8]
            
            if Header != PNG_HEADER: raise InvalidImage(f"INVALID PNG IMAGE expected {PNG_HEADER}, found: {Header}")
            
            # Chop the head.
            bytes_ = bytes_[8:]
        
        self.length = int_from_bytes(bytes_[0:4]) # 4 bytes
        bytes_      = bytes_[4:]

        self.type   = ''.join([chr(i) for i in bytes_[0:4]]) # 4 bytes
        bytes_      = bytes_[4:]

        self.data   = bytes_[0 : self.length] # chunklength bytes.
        bytes_      = bytes_[self.length:]

        self.CRC    = int_from_bytes(bytes_[0:4]) # 4 bytes
        bytes_      = bytes_[4:]
        
        return bytes_
        

class IHDR(Chunk): 
    def __init__(self):          
        super().__init__()

        self.width             = 0
        self.height            = 0        
        self.BitDepth          = 0     
        self.ColorType         = 0             
        self.CompressionMethod = 0       
        self.FilterMethod      = 0         
        self.InterlaceMethod   = 0  
       
    def Decode(self, bytes_):
        
        print(len(bytes_))
        
        self.width             = int_from_bytes(bytes_[0:4]) 
        self.height            = int_from_bytes(bytes_[4:8])       
        self.BitDepth          = bytes_[8]  
        self.ColorType         = bytes_[9]  
        self.CompressionMethod = bytes_[10]  
        self.FilterMethod      = bytes_[11]  
        self.InterlaceMethod   = bytes_[12]  
          
        pprint(self.asJSON(), indent=4)  

     
    def asJSON(self):             
        return {
            "width           ": self.width,  
            "height          ": self.height, 
            "BitDepth        ": self.BitDepth, 
            "ColorType       ": self.ColorType, 
            "CompressionMeth ": self.CompressionMethod, 
            "FilterMethod    ": self.FilterMethod, 
            "InterlaceMethod ": self.InterlaceMethod, 
        }


class PNGDECODER:
    def __init__(self, fn: str) -> None:
        self.fn     = fn
        self.Chunks = []
        self.ihdr   = IHDR()
        self.img    = None
    
    def ReadIHDR(self) -> dict:
        
        if self.img:
            bytes_ = self.img.read(PNG_HEADER_LEN + self.ihdr.length) # 13 
            self.ihdr.Decode(bytes_)
        
        return self.ihdr.asJSON()
    
    def Scan(self) -> None:
        chunk = Chunk()
        
        is_first = True

        if not self.img:
            return
        
        bytes_ = self.img.read()

        while(len(bytes_) > 0):
            bytes_ = chunk.Scan(bytes_, is_first)
            
            if chunk.type == "IHDR":
                # Decode IHDR chunk.
                self.ihdr.Decode(chunk.data)

            print(chunk.stat())
            
            if is_first: is_first = False


        
    
    def __enter__(self) -> None:
        self.img = open(self.fn, "rb+") 
        return self
    
    def __exit__(self, exc_type, exc_value, exc_tb) -> None: self.img.close()


def main(): 
    
    with PNGDECODER(FILE) as dec: dec.Scan()


if __name__ == '__main__': 
    main()
