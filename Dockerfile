FROM ubuntu:16.04
# get necessary packages
RUN apt-get update \
 && apt-get install -y \
     cmake \
     curl \
     gcc-5 \
     git \
     libgtk2.0-dev \
     libperl-dev \
     netcat \
     python2.7-minimal \
     python-pip \
     scons \
     screen \
     swig \
     tcpdump \
     vim-common \
     vim

#Download repos
RUN git clone https://github.com/pesco/hammer/ /home/hammer

#Install hammer
RUN cd /home/hammer \
 && scons \
 && scons install \
 && ldconfig

COPY Makefile m2rtu.c rtu2m.c serv.py /home/ses92/

RUN cd /home/ses92 \ 
 && make

EXPOSE 9003

ENTRYPOINT python /home/ses92/serv.py
#ENV LD_LIBRARY_PATH=/usr/local/lib
