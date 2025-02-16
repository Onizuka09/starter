# Setting a base image to install 

FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive



RUN apt-get update && apt-get install -y \
    build-essential \ 
    python3 \ 
    python3-pip \ 
    python3-setuptools \ 
    make \ 
    git \ 
    curl \ 
    libssl-dev \ 
    libcurl4-openssl-dev \ 
    pkg-config \    
    && rm -rf /var/lib/apt/lists/*




# RUN pip3 install platformio
RUN pip3 install platformio

# SET up for the pio env 
RUN platformio platform install espressif32

# set the working directory 
WORKDIR /starter 

COPY . /starter/firmware

WORKDIR /starter/firmware


# RUN bash 

CMD ["bash"]

