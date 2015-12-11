From ubuntu:14.04
MAINTAINER kruffin "kruffin@gmail.com"

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get -y -qq update > /dev/null
ADD makefile /tmp/pushover
ADD src /tmp/pushover/src

RUN make install --directory="/tmp/pushover"