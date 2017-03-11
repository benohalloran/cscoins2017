#!/bin/bash
openssl req -x509 -newkey rsa:4096 -keyout key.pem -out server.pem -nodes -days 1001
cat key.pem >> cert.pem
