#include <string>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <disk/disk.hpp>
#include <disk/disk_config.hpp>
#include <disk/partition.hpp>
#include <formats/disk_format_factory.hpp>
#include <io/stream/file_disk_stream.hpp>
#include <vfs/file_system.hpp>

namespace py = pybind11;

PYBIND11_MODULE(disklib, m) {
  py::class_<io::stream::DiskStream>(m, "DiskStream");

  py::class_<io::stream::FileDiskStream, io::stream::DiskStream>(m, "FileDiskStream")
    .def(pybind11::init<std::string&>());
    
  py::class_<disk::DiskConfig>(m, "DiskConfig")
    .def(pybind11::init<>())
    .def("setStream", &disk::DiskConfig::setStream)
    .def("getStream", &disk::DiskConfig::getStream)
    .def("setKeys", [](disk::DiskConfig& config, std::string& s) { config.setKeys(s.data(), s.length()); })
    .def("getKeys", &disk::DiskConfig::getKeys);

  py::class_<disk::Disk>(m, "Disk")
    .def("getDataProvider", &disk::Disk::getDataProvider)
    .def("getPartitions", &disk::Disk::getPartitions)
    .def("getPartitionByName", &disk::Disk::getPartitionByName)
    .def("addPartition", &disk::Disk::addPartition)
    .def("getSectorSize", &disk::Disk::getSectorSize);

  py::class_<disk::Partition>(m, "Partition")
    .def("mount", &disk::Partition::mount)
    .def("getName", &disk::Partition::getName)
    .def("setName", &disk::Partition::setName)
    .def("getVfs", &disk::Partition::getVfs)
    .def("getDataProvider", &disk::Partition::getDataProvider)
    .def("getLength", &disk::Partition::getLength)
    .def("getStart", &disk::Partition::getStart)
    .def("getEnd", &disk::Partition::getEnd);

  py::class_<formats::DiskFormatFactory>(m, "DiskFormatFactory")
    .def("detect", [](disk::DiskConfig* config) 
      { return formats::DiskFormatFactory::getInstance()->detectFormat(config); });

  py::class_<vfs::Vfs>(m, "Vfs")
    .def("mount", &vfs::Vfs::mount)
    .def("getRoot", &vfs::Vfs::getRoot)
    .def("setAdapter", &vfs::Vfs::setAdapter)
    .def("isMounted", &vfs::Vfs::isMounted);

  py::enum_<vfs::VfsNodeType>(m, "VfsNodeType")
    .value("Directory", vfs::VfsNodeType::DIRECTORY)
    .value("File", vfs::VfsNodeType::FILE)
    .export_values();

  py::class_<vfs::VfsNode>(m, "VfsNode")
    .def(pybind11::init<vfs::VfsNodeType>())
    .def("getType", &vfs::VfsNode::getType)
    .def("getName", &vfs::VfsNode::getName)
    .def("setName", &vfs::VfsNode::setName)
    .def("setParent", &vfs::VfsNode::setParent)
    .def("getLastAccessTime", &vfs::VfsNode::getLastAccessTime)
    .def("getlastModifiedTime", &vfs::VfsNode::getLastModifiedTime)
    .def("getCreationTime", &vfs::VfsNode::getCreationTime)
    .def("setLastAccessTime", &vfs::VfsNode::setLastAccessTime)
    .def("setlastModifiedTime", &vfs::VfsNode::setLastModifiedTime)
    .def("setCreationTime", &vfs::VfsNode::setCreationTime)
    .def("getOffsets", &vfs::VfsNode::getOffsets)
    .def("__str__", [](const vfs::VfsNode& node) {
      return node.getName();
    });

  py::class_<vfs::VfsDirectory, vfs::VfsNode>(m, "VfsDirectory")
    .def("addChild", &vfs::VfsDirectory::addChild)
    .def("getChildren", &vfs::VfsDirectory::getChildren)
    .def("getChildCount", &vfs::VfsDirectory::getChildCount);

  py::class_<vfs::VfsFile, vfs::VfsNode>(m, "VfsFile")
    .def("setFileSize", &vfs::VfsFile::setFileSize)
    .def("getFileSize", &vfs::VfsFile::getFileSize);
}
