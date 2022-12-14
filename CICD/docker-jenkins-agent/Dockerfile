# The MIT License
#
#  Copyright (c) 2015-2020, CloudBees, Inc. and other Jenkins contributors
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#  THE SOFTWARE.

# FIXME(oleg_nenashev): This is not an official AdoptOpenJDK Docker Image.
# There is no official Ubuntu images at the moment.
# Needs upgrade when/if there is an official ubuntu image.
FROM adoptopenjdk/openjdk8:jdk8u272-b10-ubuntu

ARG VERSION=4.6
ARG user=jenkins
ARG group=jenkins
ARG uid=1000
ARG gid=1000

RUN addgroup --gid ${gid} ${group}
RUN adduser --home /home/${user} --uid ${uid} --ingroup ${group} --disabled-password --gecos "" ${user}
LABEL Description="This is a base image, which provides the Jenkins agent executable (slave.jar)" Vendor="Jenkins project" Version="${VERSION}"

ARG AGENT_WORKDIR=/home/${user}/agent

RUN apt-get -y update \ 
  && apt-get -y install curl bash git git-lfs openssh-client openssl procps \
  && curl --create-dirs -fsSLo /usr/share/jenkins/agent.jar https://repo.jenkins-ci.org/public/org/jenkins-ci/main/remoting/${VERSION}/remoting-${VERSION}.jar \
  && chmod 755 /usr/share/jenkins \
  && chmod 644 /usr/share/jenkins/agent.jar \
  && ln -sf /usr/share/jenkins/agent.jar /usr/share/jenkins/slave.jar
  
USER ${user}
ENV AGENT_WORKDIR=${AGENT_WORKDIR}
RUN mkdir /home/${user}/.jenkins && mkdir -p ${AGENT_WORKDIR}

VOLUME /home/${user}/.jenkins
VOLUME ${AGENT_WORKDIR}
WORKDIR /home/${user}
