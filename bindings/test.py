import os
import sys
import disklib

# TODO: Write actual tests
def main(imagePath, keyPath):
  stream = disklib.FileDiskStream(imagePath)
  keys = open(keyPath,'rb').read()
  config = disklib.DiskConfig()
  config.setStream(stream)
  config.setKeys(keys)
  disk = disklib.DiskFormatFactory.detect(config)
  partition = disk.getPartitionByName('dev_hdd0')
  print(partition.getStart())
  print(partition.getEnd())
  print(partition.getLength())
  vfs = partition.getVfs()
  vfs.mount()
  root = vfs.getRoot()
  for node in root.getChildren():
    print(node.getName())

if __name__ == "__main__":
  main(sys.argv[1], sys.argv[2])
