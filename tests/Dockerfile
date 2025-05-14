FROM debian:bookworm-slim

WORKDIR /tests

RUN apt-get update && apt-get install -y \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

COPY scripts/ .
RUN chmod +x *.sh

COPY ../ft_ls .
RUN ls -la

RUN ./Filetypes.sh && ./Oldfile.sh

ENTRYPOINT ["bash"]