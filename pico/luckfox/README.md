# pico

## setup build
```bash
meson setup .build \
  --buildtype=release \
  --cross-file luckfox-cross.ini
```

## compile
```bash
scripts/format.sh && meson compile -C .build
```

### configure device ssl certificate verification
```bash
curl --etag-compare etag.txt \
  --etag-save etag.txt \
  --remote-name \
  https://curl.se/ca/cacert.pem
cp -v cacert.pem /etc/ssl/certs/ca-certificates.crt
```
