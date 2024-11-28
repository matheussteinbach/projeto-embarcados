const sqlite3 = require('sqlite3');
const express = require('express');
const bodyParser = require('body-parser');

const db = new sqlite3.Database('./dados.db', (err) => {
  if (err) {
    console.log('Erro ao acessar o banco de dados de controle.');
    throw err;
  }
  console.log('Conectado ao banco de dados de controle.');
});

db.run(
  `
    CREATE TABLE IF NOT EXISTS controle (
      codigo INT UNIQUE NOT NULL,
      valor_limite NUMERIC(10) NOT NULL,
      PRIMARY KEY (codigo)
    )
  `,
  [],
  (err) => {
    if (err) {
      console.log('Erro ao criar tabela de controle.');
      throw err;
    }
  }
);

db.get('SELECT (count(1) > 0) AS existe FROM controle', [], (err, result) => {
  if (!result.existe) {
    db.run('INSERT INTO controle VALUES (1, 0)');
  }
});

const app = express();
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended: true}));

app.get('/controle', (req, res, next) => {
  db.get('SELECT valor_limite FROM controle WHERE codigo = ?', [1], (err, result) => {
    if (err) {
      res.status(500).send('Ocorreu um erro ao obter a configuração.');
      console.log('ERRO: ' + err.message);
      return;
    }
    if (!result) {
      res.status(404).send('A configuração de controle não foi encontrada.');
      return;
    }
    res.status(200).send(result.valor_limite.toString());
  })
});

app.patch('/controle', (req, res, next) => {
  db.run(
    'UPDATE controle SET valor_limite = ? WHERE codigo = ?',
    [req.body.valor_limite, 1],
    function (err) {
      if (err) {
        res.status(500).send('Ocorreu um erro ao atualizar a configuração.');
        console.log('ERRO: ' + err.message);
        return;
      }
      if (this.changes == 0) {
        res.status(404).send('A configuração de controle não foi encontrada.');
        return;
      }
      res.status(200).send('Configuração atualizada com sucesso!');
    }
  )
});

const porta = 9010;
app.listen(porta, () => console.log('API de controle UP'));
