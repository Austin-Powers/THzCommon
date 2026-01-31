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
- __ConfigurationProject__ `struct` `<configuration.hpp>` Name provider for the configuration project.
- __Configuration__ `class` `<configuration.hpp>` Manages access to a tree like configuration structure.
  
- __ConfigurationBuilder__ `class` `<configurationbuilder.hpp>` Can be used to create example configurations.
  
- __ConfigurationStorage__ `class` `<configurationstorage.hpp>` Loads a configuration from a file or memorybuffer and provides it to the rest of the application.
  

### Converter
- __Encoder__ `class` `<base64.hpp>` Helps encode data into base64.
- __Decoder__ `class` `<base64.hpp>` Helps decode data from base64.
  
- __Encoder__ `class` `<huffmancoder.hpp>` Encapsulated the code for performing a Huffman-Encoding.
- __Decoder__ `class` `<huffmancoder.hpp>` Encapsulated the code for performing a Huffman-Decoding.
  
- __HuffmanProject__ `struct` `<huffmancommons.hpp>` Name provider for the Huffman project.
- __CodeHeader__ `struct` `<huffmancommons.hpp>` The header of a table of Huffman codes.
- __SymbolDistribution__ `definition` `<huffmancommons.hpp>` The type for a symbol distribution.
- __CodeTable__ `class` `<huffmancommons.hpp>` Encapsulates the code table for the Huffman-Coding.
  

### Diagnostics
- __HexView__ `class` `<hexview.hpp>` Viewer to conveniently display binary data on the console and export it human readable to files.
  
- __StopWatch__ `class` `<stopwatch.hpp>` Class for measuring the duration of certain processes and operations.
  

### Logging
- __LogLevel__ `enum` `<logging.hpp>` Enumeration of the different log levels.
- __Project__ `concept` `<logging.hpp>` Concept for ProjectClass, able to provide a name to the Logger.
- __LoggingProject__ `struct` `<logging.hpp>` Name provider for the logging project.
- __Logger__ `class` `<logging.hpp>` Logs messages to console and specified log file.
  

### Math
- __BilinearInterpolation__ `class` `<bilinearInterpolation.hpp>` Template class encapsulating the algorithm for bilinear interpolation.
  
- __Direction__ `enum` `<constants.hpp>` Directions for geometric operations.
  
- __MatrixProject__ `struct` `<matrix.hpp>` Name provider for the THzCommon.Math.Matrix project.
- __Matrix__ `class` `<matrix.hpp>` Represents a Matrix.
  
- __Point__ `class` `<point.hpp>` A point in 2 dimensional space.
  
- __Rectangle__ `class` `<rectangle.hpp>` A rectangle with basic geometrical methods.
  

### Memory
- __IMemoryPool__ `class` `<imemorypool.hpp>` Interface for all memory pools.
  

### Network
- __Address__ `struct` `<address.hpp>` Combines IP address and port.
  
- __IPVersion__ `enum` `<common.hpp>` Enumerates the version of the IP protocol.
- __Protocol__ `enum` `<common.hpp>` The type of protocol building on top of the internet protocol.
- __IPAddress;__ `struct` `<common.hpp>` 
- __IPAddress<IPVersion::V4>__ `struct` `<common.hpp>` 
- __IPAddress<IPVersion::V6>__ `struct` `<common.hpp>` 
- __SocketApi__ `class` `<common.hpp>` Encapsulates the native socket API.
  
- __SocketBase__ `class` `<socketbase.hpp>` Base containing shared functionality of all sockets.
  
- __TCPConnection__ `class` `<tcpconnection.hpp>` Wrapper for handling a TCP connection.
  
- __TCPSocket__ `class` `<tcpsocket.hpp>` Encapsulates necesarry code to send and receive data via Transmission Control Protocol (TCP).
  
- __UDPSocket__ `class` `<udpsocket.hpp>` Encapsulates necesarry code to send and receive data via User Datagram Protocol (UDP).
  

### Random
- __Ant__ `class` `<ant.hpp>` Uses Langtons-Ant as a source of random numbers.
  

### Structures
- __Octree__ `class` `<octree.hpp>` Implementation of an octree based on a cube shaped space.
  
- __Queue__ `class` `<queue.hpp>` Template implementation of a static sized queue.
  
- __Stack__ `class` `<stack.hpp>` Template implementation of a static sized stack.
  

### Utility
- __BitBufferReader__ `class` `<bitbuffer.hpp>` Encapsulates the code for reading bitwise from a byte buffer.
- __BitBufferWriter__ `class` `<bitbuffer.hpp>` Encapsulates the code for writing bitwise from a byte buffer.
  
- __MultiplyableBy__ `concept` `<concepts.hpp>` Concept of a type that can be multiplied by another type.
- __MultiplicationResult__ `struct` `<concepts.hpp>` Deduces the return type of the multiplication of the two given types. 
- __MultiplicationResult_t__ `definition` `<concepts.hpp>` Shortcut to MultiplicationResult::type
- __Container__ `concept` `<concepts.hpp>` Concept of a container a span can be created from.
  
- __LineSequencer__ `class` `<lineSequencer.hpp>` Class for accessing a buffer line by line.
  
- __Range2D__ `class` `<range2D.hpp>` Generator for indexing a buffer representing 2D data using a for-each loop.
  
- __Range2DFolding__ `class` `<range2DFolding.hpp>` Generator for folding the content of a buffer representing 2D data using 2 nested for-each loops.
  
- __errno_t__ `definition` `<result.hpp>` The type returned by calling errno.
- __Result__ `class` `<result.hpp>` Can be returned by function calls that might fail with an error code.
- __Result<errno_t>__ `class` `<result.hpp>` Specialization in case T is identical to errno_t.
  
- __StaticPImpl__ `class` `<staticPImpl.hpp>` Helper class for using static memory for the pointer-to-implementation-idiom.
  
- __Character__ `concept` `<stringhelpers.hpp>` Concept of a character type.
  
- __StringViewTokenizer__ `class` `<stringviewhelpers.hpp>` Enables easy tokenizing of string_view instances.
  
- __SystemNow__ `definition` `<time.hpp>` Shortcut to the system_clock::now function.
- __Milliseconds__ `definition` `<time.hpp>` Shortcut for milliseconds.
- __SystemTimePoint__ `definition` `<time.hpp>` Shortcut for the system_clock time_point.
  
- __WorkerThread__ `struct` `<workerThread.hpp>` Instances and code common for running a worker thread.
  

