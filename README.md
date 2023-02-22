# Implementation of TCP Client

EPITA - Week2 - Programming

![Workflow](https://github.com/ProtossDragoon/epita-w2-prog-tcpclient/actions/workflows/master.yaml/badge.svg)

## Quickstart

`terminal`
```bash
git clone https://github.com/ProtossDragoon/epita-w2-prog-tcpclient.git
cd epita-w2-prog-tcpclient
make
./main example.com
```

`output`
```
HTTP/1.0 200 OK
Age: 258187
Cache-Control: max-age=604800
Content-Type: text/html; charset=UTF-8
Date: Wed, 22 Feb 2023 03:13:15 GMT
Etag: "3147526947+ident"
Expires: Wed, 01 Mar 2023 03:13:15 GMT
Last-Modified: Thu, 17 Oct 2019 07:18:26 GMT
Server: ECS (dcb/7FA5)
Vary: Accept-Encoding
X-Cache: HIT
Content-Length: 1256
Connection: close

<!doctype html>
<html>
<head>
    <title>Example Domain</title>

    <meta charset="utf-8" />
    <meta http-equiv="Content-type" content="text/html; charset=utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <style type="text/css">
    body {
        background-color: #f0f0f2;
        margin: 0;
        padding: 0;
        font-family: -apple-system, system-ui, BlinkMacSystemFont, "Segoe UI", "Open Sans", "Helvetica Neue", Helvetica, Arial, sans-serif;
        
    }
    div {
        width: 600px;
        margin: 5em auto;
        padding: 2em;
        background-color: #fdfdff;
        border-radius: 0.5em;
        box-shadow: 2px 3px 7px 2px rgba(0,0,0,0.02);
    }
    a:link, a:visited {
        color: #38488f;
        text-decoration: none;
    }
    @media (max-width: 700px) {
        div {
            margin: 0 auto;
            width: auto;
        }
    }
    </style>    
</head>

<body>
<div>
    <h1>Example Domain</h1>
    <p>This domain is for use in illustrative examples in documents. You may use this
    domain in literature without prior coordination or asking for permission.</p>
    <p><a href="https://www.iana.org/domains/example">More information...</a></p>
</div>
</body>
</html>
```
