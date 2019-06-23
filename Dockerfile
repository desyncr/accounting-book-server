FROM gcc:4.9

EXPOSE 8080

COPY . /usr/src/server
WORKDIR /usr/src/server

RUN make
CMD ["make", "run"]
