FROM ubuntu:18.04

ENV LANG C.UTF-8

RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install g++ valgrind -y
RUN apt-get install --reinstall make
RUN apt-get install pkg-config -y
RUN apt-get install libgtk-3-dev -y
RUN export LD_LIBRARY_PATH=/usr/include/gtk-3.0/


ENV APP_HOME /ubuntu

RUN mkdir $APP_HOME
WORKDIR $APP_HOME

ADD . $APP_HOME
CMD ["bash"]