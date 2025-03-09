# computepipeline
This code contains an implementation of ComputePipeline which is responsible for performing a series of transformations to
achieve a final result
The initial action is always a load of an item from storage which load action is required, depends on the given initial uri , which
may be a file path starting with file:// , a fully qualified URL ( http:// or
https:// ) or a path pointing to the application bundle ( bundle:// )
The item must then be further processed by different actions, depending on
its type:
a. Images must be decoded
b. Compressed data must be decompressed
c. JSON must be turned into a C++ object

Actions are following factory design pattern for creating and registering new actions
computePipeline has a centralized manager to make sure pipelines are not created more than once for the same uri or resource for performance reasons.
It also store the last successful action result just in case the pipeline was interrupted and we don't want to resume from scratch because actions might be expensive (e.g loading uri action).


Build Instructions:
git clone https://github.com/Malek-ATIA/computepipeline.git
cd computepipeline
mkdir build && cd build
cmake
make

Finally you can run ./ComputePipeline


