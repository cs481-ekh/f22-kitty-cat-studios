#!/bin/bash

java -jar /usr/share/jenkins/agent.jar -jnlpUrl http://ubuntu-broncodrome:8080/computer/UBUNTU%20BUILDER/slave-agent.jnlp -secret 98c9d26fb92c16038fcc7bed5a5771fab680f81580f7885caa653447ad1b5582 -workDir "/home/jenkins"
