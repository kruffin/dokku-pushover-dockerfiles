From ubuntu:14.04
MAINTAINER kruffin "kruffin@gmail.com"

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get -y -qq update > /dev/null
RUN apt-get -y -qq install nodejs > /def/null
ADD makefile /tmp/pushover
ADD node-source /tmp/pushover

RUN npm install --global /tmp/pushover