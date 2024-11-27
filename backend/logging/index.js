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

// API

const porta = 9020;
app.listen(porta, () => console.log('API de logging UP'));
