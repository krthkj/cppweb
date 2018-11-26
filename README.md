# CPP WEB
sample project to understand building library & compiling C++ program using docker and connecting to mongoDB.

Reference:
https://www.linkedin.com/learning/web-servers-and-apis-using-c-plus-plus/adding-the-mongodb-c-plus-plus-drivers

# Stage 1: build simple cpp
## Step 1: First docker file build
cd cppweb/cppbox
docker build -t cppbox .

## Step 2: add volume to container
add following option while using docker to add volume
`-v ~/git/cppweb:/usr/src/cppweb`

## Step 3: Build Project from docker container
hostmachine $      cd cppweb
hostmachine $      docker run -v ~/git/cppweb:/usr/src/cppweb -ti cppbox:latest bash
dockerContainer $  cd /usr/src/cppweb
dockerContainer $  cd hello_crow/build
dockerContainer $  cmake ..
dockerContainer $  make

## Step 4: Run executable using container
cd cppweb/hello_crow/build
docker run -v ~/git/cppweb:/usr/src/cppweb -p 8080:8080 -e PORT=8080 cppbox:latest /usr/src/cppweb/hello_crow/build/hello_crow

here, -p refers to port assignment
      -e refers to Env variable

# Stage 2: build project using supporting library
## Step 1: create Docker and Build mongodb C++ library (need to do it once)
create Docker file, cppweb/bbox/Dockerfile
cd cppweb/bbox
docker build --rm --squash --no-cache -t bbox:latest .

## Step 2: update root Dockerfile
update the docker file, cppweb/hello_crow/Dockerfile
update cppweb/hello_crow/CMakeLists.txt

## Step 2: Build project
cd cppweb/hello_crow
docker build --rm --squash --no-cache -t hello_crow:latest .

# Step 3 : run executable
cd cppweb/hello_crow
docker run -p 8080:8080 -e PORT=8080 -e MONGODB_URI="mongodb://10.0.1.27:27017/musicDB" hello_crow:latest
