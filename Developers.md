# Developer's Guide
This guide serves to help developers make their own tools using the core library.
- [Set Up](#set-up)
- [Accessing Partitions](#accessing-partitions)
- [Accessing a Partition's file system](#accessing-a-partitions-file-system)
  - [Directory Attributes](#VfsDirectory)
  - [File Attributes](#VfsFile)
  - [Common Attributes](#VfsNode)
- [Accessing Raw Data](#accessing-raw-data)

## Set Up
First we need to set up a `DiskConfig`. We create a `FileDiskStream` which, internally, just wraps an `std::ifstream` to provide a stream that the library will read raw, unprocesssed data from a file. Then we load our keys file to memory and pass that to the `DiskConfig` as well.
```cpp
FileDiskStream* stream = new FileDiskStream("c:/path/to/hdd/image.img");
/* You need to implement readFile, or get keys in whatever way you want to. */
std::vector<char> keys = readFile("c:/path/to/hdd/keys.bin");

DiskConfig config;
config.setStream(stream);
config.setKeys(keys.data(), keys.size());
```

Then, we can use our `DiskConfig` to pass to `DiskFormatFactory::detect()`. This function will automatically check if it is a recognized format and if it is, it will load the partition table, set up crypto, and returns a `Disk` object which will be our entry point for accessing data.
```cpp
Disk* disk = DiskFormatFactory::getInstance()->detect(&config);
```

## Accessing Partitions
The `Disk` has a list of partitions that you can get by name:
```cpp
Partition* partition = disk->getPartitionByName("dev_hdd0");
```

Or you can request all partitions using `disk->getPartitions()`.

## Accessing a Partition's file system
You need to mount the file system before you are able to access it. To do so, request the `Vfs` associated with a partition, then call it's `mount()` method. After you mount it, be sure to check if it succeeded by calling `isMounted()`. After successfully mounting, you can access the root of the file system by using `getRoot()`, which returns a `VfsDirectory` that contains files at the root of the file system.

**Note:** Mounting the file system involves indexing all files and all data that each file references. Plan for the possibility that it may take some extra time to mount.
```cpp
/* Vfs provides access to the file system */
Vfs* vfs = partition->getVfs();
vfs->mount();
if (!vfs->isMounted())
  throw "Failed to mount the file system!";
VfsDirectory* root = vfs->getRoot();
```

`VfsDirectory` represents a directory, and you can access it's children by calling it's `getChildren()` method. Note that it returns a vector of `VfsNode`s. These nodes can be either a `VfsDirectory` or a `VfsFile` which represent a directory or a file respectively. You can see which type it is by calling the node's `getType()` method which returns either `VfsNodeType::DIRECTORY` or `VfsNodeType::FILE`.

**Note:** In python, you can also just use `isinstance(node, VfsDirectory)` or `isinstance(node, VfsFile)` instead of `getType()`.
```cpp
std::vector<VfsNode*> children = root->getChildren();
for (auto child in children) {
  if (child->getType() == VfsNodeType::DIRECTORY)
    cout << child->getName() << " is a directory" << endl;
  else (child->getType() == VfsNodeType::FILE)
    cout << child->getName() << " is a file" << endl;
}
```


`VfsDirectory` and `VfsFile` have their own special accessor methods.
### VfsDirectory
|Method|Description|
|------|-----------|
|getChildren|Get all node's in this directory|
|getChildCount|Get number of node's in this directory|

### VfsFile
|Method|Description|
|------|-----------|
|getFileSize|Get the size of this file's contents|

They share these methods:
### VfsNode
|Method|Description|
|------|-----------|
|getType|Get the node's type (Directory or File)|
|getName|Get the name of this node|
|getLastAccessTime|Get the time this node was last accessed|
|getLastModifiedTime|Get the time this node was last modified|
|getCreationTime|Get the time this node was created|
|getOffsets|Get data offsets referenced to by this node|

## Accessing raw data
You can access raw data by using `DataProvider`s. Both the `Disk` and `Partition` classes have their own `DataProvider`s. The `Disk` class has access to the entire disk, while the `Partition` class only has access to the range it exists in. The `DataProvider` will take care of decrypting sectors (even multiple layers) for you.
```cpp
DataProvider* reader;
if (weWantDiskReader)
  reader = disk->getDataProvider();
else
  reader = partition->getDataProvider();
reader->seek(0, /*whence=*/0);
const auto kSize = 0x1000;
vector<char> buffer(kSize);
auto amountRead = reader->read(buffer.data(), kSize);
if (amountRead != kSize)
  throw "Failed to read requested amount of data!";
// Do something with buffer...
```
