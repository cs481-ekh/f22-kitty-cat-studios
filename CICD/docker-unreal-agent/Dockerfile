FROM ubuntu-agent:latest

USER root
RUN apt update -y && apt install -y make build-essential

WORKDIR /home/jenkins

USER jenkins

COPY ./entrypoint.sh .

ENTRYPOINT ["bash"]
