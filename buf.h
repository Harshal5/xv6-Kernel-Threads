struct buf {
  int flags;
  uint dev;   // because one device driver can handle multiple devices of same type   // stores the harddisk number it read from
  uint blockno;   // storing the block number that has beem read from the disk
  struct sleeplock lock;
  uint refcnt;
  struct buf *prev; // LRU cache list // LRU == Least recently used  
  struct buf *next;
  struct buf *qnext; // disk queue
  uchar data[BSIZE];    // array of size 512, this is where the actual data is cached
};
#define B_VALID 0x2  // buffer has been read from disk
#define B_DIRTY 0x4  // buffer needs to be written to disk

