const sqlite3 = require('sqlite3');
const express = require('express');
const bodyParser = require('body-parser');

const db = new sqlite3.Database('./dados.db', (err) => {
  if (err) {
    console.log('Erro ao acessar o banco de dados de logging.');
    throw err;
  }
  console.log('Conectado ao banco de dados de logging.');
});

db.run(
  `
    CREATE TABLE IF NOT EXISTS logging (
      codigo INT UNIQUE NOT NULL,
      valor_lido NUMERIC(10) NOT NULL,
      data_leitura DATETIME NOT NULL,
      PRIMARY KEY (codigo)
    )
  `,
  [],
  (err) => {
    if (err) {
      console.log('Erro ao criar tabela de logging.');
      throw err;
    }
  }
);

const app = express();
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended: true}));

app.get('/logging', (req, res, next) => {
  db.all('SELECT * FROM logging', [], (err, result) => {
    if (err) {
      res.status(500).send('Ocorreu um erro ao obter os logs.');
      console.log('ERRO: ' + err.message);
      return;
    }
    res.status(200).json(result);
  })
});

app.post('/logging', (req, res, next) => {
  db.run(
    "INSERT INTO logging VALUES ((SELECT COALESCE(MAX(codigo), 0) + 1 FROM logging), ?, DATETIME('now'))",
    [req.body.valor_lido],
    function (err) {
      if (err) {
        res.status(500).send('Ocorreu um erro ao inserir o log.');
        console.log('ERRO: ' + err.message);
        return;
      }
      res.status(200).send('Log inserido com sucesso!');
    }
  )
});

app.delete('/logging/limpar', (req, res, next) => {
  db.run(
    "DELETE FROM logging WHERE 1=1",
    [],
    function (err) {
      if (err) {
        res.status(500).send('Ocorreu um erro ao limpar a base de logs.');
        console.log('ERRO: ' + err.message);
        return;
      }
      res.status(200).send('Base de logs limpa com sucesso!');
    }
  )
});

const porta = 9020;
app.listen(porta, () => console.log('API de logging UP'));
