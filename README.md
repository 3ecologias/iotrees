/***********************************************************************
/*    This program is free software: you can redistribute it and/or modify
/*    it under the terms of the GNU General Public License as published by
/*    the Free Software Foundation, either version 3 of the License, or
/*    (at your option) any later version.
/*
/*    This program is distributed in the hope that it will be useful,
/*    but WITHOUT ANY WARRANTY; without even the implied warranty of
/*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/*    GNU General Public License for more details.
/*
/*    You should have received a copy of the GNU General Public License
/*    along with this program.  If not, see <http://www.gnu.org/licenses/>


Dendrometro de precisão para mensurar tamanho inicial e variações de crescimento do diâmetro das árvores (DAP - diametro a altura do peito com casca).

Code by 3ecologias.net <root@3ecologias.net>

   * 1 lap lapV =~ 0.5 volts (v) vcc 5v
   * 1 lap lapC =~ 7.9 cm
   * perimetro extra cE =~ 4 cm
   * perimetro total cT =~ 64 cm
   * diametro inicial dI =~ 1.2732 cm
   * diametro final dT =~ 20.3718 cm
   * led vermelho: pre-calibragem 
   * led verde: modo de cálculo do diâmetro (DAP) e do crescimento (ICAd) em cm.
   
   steps:
   1. ajuste o carretel do sensor e acople o dispositivo na árvore com o dispositivo desligado;
   2. ligue o dispositivo para habilitar o modo de calibragem, medição inicial do diâmetro, crescimento e catalogo de dados;
   3. o modo de calibragem do sensor verifica se o carretel realizou no mímino uma volta completa;
   4. o modo de medição do DAP e ICAd captura por 20 segundos o valor inicial do diâmetro;
   5. o modo de crescimento coleta, a cada minuto, o valor do diâmetro (DAP) e compara com o último valor do DAP para registrar o incremento do diâmetro (ICAd);
   6. o modo de catalogo armazena, a cada minuto, um log em CSV no SDCard com os valores do DAP: inicial, final e o ICAd. 
   7. Para salvar o log, basta voltar o carretel para a posição inicial e aguardar o led vermelho acender. Depois disso, desligue o dispositivo e remova o SDCard.

   info:
   a pasta printed-circuit/ contém os arquivos PDF para impressão de circuito impresso, o arquivo original para edição pelo Fritizing (http://fritzing.org/), o modelo de montagem de protoboard e o arquivo com a lista de componentes necessários.

   a pasta string-pot-am-2674/ contém os arquivos modelados do sensor para a impressão 3D, fotos de exemplo e lista de materiais para o sensor.

   obs: o potenciômetro utilizado aparece nas duas listagens, embora apenas um potênciometro seja utilizado. O objetivo é que as duas listas de materiais sejam independentes e modulares.


