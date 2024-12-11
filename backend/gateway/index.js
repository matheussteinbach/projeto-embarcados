const httpProxy = require('express-http-proxy');
const express = require('express');
const logger = require('morgan');
const cors = require('cors');
const app = express();

app.use(logger('dev'));
app.use(cors());

const DOMINIO_BASE = 'http://localhost';

const selectProxyHost = (req) => {
  if (req.path.startsWith('/controle')) {
    return DOMINIO_BASE + ':9010/';
  }
  if (req.path.startsWith('/logging')) {
    return DOMINIO_BASE + ':9020/';
  }
  return null;
}

app.use((req, res, next) => {
  const proxy = selectProxyHost(req);
  if (!proxy) {
    res.status(404).send('Not found');
    return;
  }
  httpProxy(proxy)(req, res, next);
});

const porta = 9000;
app.listen(porta, () => console.log('Backend UP'));
