# THzCommon
Collection of classes, functions and templates to be used by other projects.

## Contents
- __Converter__ Classes and functions for converting data
- __Diagnostics__ Classes runtime diagnostics
- __Logging__ Contains the Logger class for logging events in other projects
- __Math__ Common math related classes and functions
- __Memory__ Classes for memory handling
- __Network__ Classes for network communication
- __Structures__ Data handling structures and algorithms
- __Utility__ Utility classes and functions

## Details
Below is more detailed information about the contents of each subdirectory.

### Configuration
- __`struct ConfigurationProject`__ _(configuration.hpp)_ Name provider for the configuration project.
- __`class Configuration`__ _(configuration.hpp)_ Manages access to a tree like configuration structure.
  
- __`class ConfigurationBuilder`__ _(configurationbuilder.hpp)_ Can be used to create example configurations.
  
- __`class ConfigurationStorage`__ _(configurationstorage.hpp)_ Loads a configuration from a file or memorybuffer and provides it to the rest of the application.
  

### Converter
- __`class Encoder`__ _(base64.hpp)_ Helps encode data into base64.
- __`class Decoder`__ _(base64.hpp)_ Helps decode data from base64.
  
- __`class Encoder`__ _(huffmancoder.hpp)_ Encapsulated the code for performing a Huffman-Encoding.
- __`class Decoder`__ _(huffmancoder.hpp)_ Encapsulated the code for performing a Huffman-Decoding.
  
- __`struct HuffmanProject`__ _(huffmancommons.hpp)_ Name provider for the Huffman project.
- __`struct CodeHeader`__ _(huffmancommons.hpp)_ The header of a table of Huffman codes.
- __`definition SymbolDistribution`__ _(huffmancommons.hpp)_ The type for a symbol distribution.
- __`class CodeTable`__ _(huffmancommons.hpp)_ Encapsulates the code table for the Huffman-Coding.
  

### Diagnostics
- __`class HexView`__ _(hexview.hpp)_ Viewer to conveniently display binary data on the console and export it human readable to files.
  
- __`class StopWatch`__ _(stopwatch.hpp)_ Class for measuring the duration of certain processes and operations.
  

### Logging
- __`enum LogLevel`__ _(logging.hpp)_ Enumeration of the different log levels.
- __`concept Project`__ _(logging.hpp)_ Concept for ProjectClass, able to provide a name to the Logger.
- __`struct LoggingProject`__ _(logging.hpp)_ Name provider for the logging project.
- __`class Logger`__ _(logging.hpp)_ Logs messages to console and specified log file.
  

### Math
- __`class BilinearInterpolation`__ _(bilinearInterpolation.hpp)_ Template class encapsulating the algorithm for bilinear interpolation.
  
- __`enum Direction`__ _(constants.hpp)_ Directions for geometric operations.
  
- __`struct MatrixProject`__ _(matrix.hpp)_ Name provider for the THzCommon.Math.Matrix project.
- __`class Matrix`__ _(matrix.hpp)_ Represents a Matrix.
  
- __`class Point`__ _(point.hpp)_ A point in 2 dimensional space.
  
- __`class Rectangle`__ _(rectangle.hpp)_ A rectangle with basic geometrical methods.
  

### Memory
- __`class IMemoryPool`__ _(imemorypool.hpp)_ Interface for all memory pools.
  

### Network
- __`struct Address`__ _(address.hpp)_ Combines IP address and port.
  
- __`enum IPVersion`__ _(common.hpp)_ Enumerates the version of the IP protocol.
- __`enum Protocol`__ _(common.hpp)_ The type of protocol building on top of the internet protocol.
- __`struct IPAddress;`__ _(common.hpp)_ 
- __`struct IPAddress<IPVersion::V4>`__ _(common.hpp)_ 
- __`struct IPAddress<IPVersion::V6>`__ _(common.hpp)_ 
- __`class SocketApi`__ _(common.hpp)_ Encapsulates the native socket API.
  
- __`class SocketBase`__ _(socketbase.hpp)_ Base containing shared functionality of all sockets.
  
- __`class TCPConnection`__ _(tcpconnection.hpp)_ Wrapper for handling a TCP connection.
  
- __`class TCPSocket`__ _(tcpsocket.hpp)_ Encapsulates necesarry code to send and receive data via Transmission Control Protocol (TCP).
  
- __`class UDPSocket`__ _(udpsocket.hpp)_ Encapsulates necesarry code to send and receive data via User Datagram Protocol (UDP).
  

### Random
- __`class Ant`__ _(ant.hpp)_ Uses Langtons-Ant as a source of random numbers.
  

### Structures
- __`class Octree`__ _(octree.hpp)_ Implementation of an octree based on a cube shaped space.
  
- __`class Queue`__ _(queue.hpp)_ Template implementation of a static sized queue.
  
- __`class Stack`__ _(stack.hpp)_ Template implementation of a static sized stack.
  

### Utility
- __`class BitBufferReader`__ _(bitbuffer.hpp)_ Encapsulates the code for reading bitwise from a byte buffer.
- __`class BitBufferWriter`__ _(bitbuffer.hpp)_ Encapsulates the code for writing bitwise from a byte buffer.
  
- __`concept MultiplyableBy`__ _(concepts.hpp)_ Concept of a type that can be multiplied by another type.
- __`struct MultiplicationResult`__ _(concepts.hpp)_ Deduces the return type of the multiplication of the two given types. 
- __`definition MultiplicationResult_t`__ _(concepts.hpp)_ Shortcut to MultiplicationResult::type
- __`concept Container`__ _(concepts.hpp)_ Concept of a container a span can be created from.
  
- __`class LineSequencer`__ _(lineSequencer.hpp)_ Class for accessing a buffer line by line.
  
- __`class Range2D`__ _(range2D.hpp)_ Generator for indexing a buffer representing 2D data using a for-each loop.
  
- __`class Range2DFolding`__ _(range2DFolding.hpp)_ Generator for folding the content of a buffer representing 2D data using 2 nested for-each loops.
  
- __`definition errno_t`__ _(result.hpp)_ The type returned by calling errno.
- __`class Result`__ _(result.hpp)_ Can be returned by function calls that might fail with an error code.
- __`class Result<errno_t>`__ _(result.hpp)_ Specialization in case T is identical to errno_t.
  
- __`class StaticPImpl`__ _(staticPImpl.hpp)_ Helper class for using static memory for the pointer-to-implementation-idiom.
  
- __`concept Character`__ _(stringhelpers.hpp)_ Concept of a character type.
  
- __`class StringViewTokenizer`__ _(stringviewhelpers.hpp)_ Enables easy tokenizing of string_view instances.
  
- __`definition SystemNow`__ _(time.hpp)_ Shortcut to the system_clock::now function.
- __`definition Milliseconds`__ _(time.hpp)_ Shortcut for milliseconds.
- __`definition SystemTimePoint`__ _(time.hpp)_ Shortcut for the system_clock time_point.
  
- __`struct WorkerThread`__ _(workerThread.hpp)_ Instances and code common for running a worker thread.
  

