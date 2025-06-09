FROM debian:bookworm-slim

WORKDIR /tests

RUN apt-get update && apt-get install -y \
    build-essential \
    bash \
    python3 \
    sudo \
    ncurses-bin \
    acl attr \
    && rm -rf /var/lib/apt/lists/*

# Add sudo rights and disable password
RUN echo 'ALL ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers

ENV TERM=xterm-256color

COPY scripts/docker/*.sh .
RUN chmod +x *.sh

COPY scripts/run_scripts.sh .
RUN chmod +x run_scripts.sh

COPY myls .
RUN chmod +x myls

RUN ls -la && pwd

ENTRYPOINT ["./run_scripts.sh"]