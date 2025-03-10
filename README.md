# computepipeline <br/>
This code contains an implementation of ComputePipeline which is responsible for performing a series of transformations to
achieve a final result. <br/>
The initial action is always a load of an item from storage which load action is required, depends on the given initial uri , which may be a file path starting with file:// , a fullyqualified URL ( http:// or https:// ) or a path pointing to the application bundle ( bundle:// ) The item must then be further processed by different actions, depending on its type: <br/>
a. Images must be decoded <br/>
b. Compressed data must be decompressed <br/>
c. JSON must be turned into a C++ object <br/>
 <br/>
 <br/>
Actions are following factory design pattern for creating and registering new actions <br/>
computePipeline has a centralized manager to make sure pipelines are not created more than once for the same uri or resource for performance reasons. <br/>
It also store the last successful action result just in case the pipeline was interrupted and we don't want to resume from scratch because actions might be expensive (e.g loading uri action). <br/>
 <br/>
 <br/>
# Build Instructions: <br/>
git clone https://github.com/Malek-ATIA/computepipeline.git <br/>
cd computepipeline <br/>
mkdir build && cd build <br/>
cmake .. <br/>
make <br/>
 <br/>
Finally you can run ./ComputePipeline <br/>


