FROM debian:bookworm-slim

WORKDIR /tests

RUN apt-get update && apt-get install -y \
    build-essential \
    bash \
    python3 \
    sudo \
    ncurses-bin \
    && rm -rf /var/lib/apt/lists/*

# Add sudo rights and disable password
RUN echo 'ALL ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers

ENV TERM=xterm-256color

COPY scripts/ .
RUN chmod +x *.sh

COPY myls .
RUN chmod +x myls

RUN ls -la && pwd

# Uncomment this after fixing the dependencies
# RUN bash ./Filetypes.sh && ./Oldfile.sh

ENTRYPOINT ["./Filetypes.sh"]
# ENTRYPOINT [ "bash" ]