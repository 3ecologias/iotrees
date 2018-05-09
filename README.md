/*  Dendrometro de precisão para mensurar tamanho inicial e 
 *   variações de crescimento do diâmetro das árvores 
 *   (DAP - diametro a altura do peito com casca). 
 *  
 *  Code by 3ecologias.net <root@3ecologias.net>
 *  
 *  notes:
 *  1 lap lapV =~ 0.5 volts (v) vcc 5v
 *  1 lap lapC =~ 7.9 cm
 *  perimetro extra cE =~ 4 cm
 *  perimetro total cT =~ 64 cm
 *  diametro inicial dI =~ 1.2732 cm
 *  diametro final dT =~ 20.3718 cm
 *  led vermelho: pre-calibragem 
 *  led verde: modo de cálculo do diâmetro (DAP) e do crescimento (ICAd) em cm.
 *  
 *  steps:
 *  1. ajuste o carretel do sensor e acople o dispositivo na árvore com o dispositivo desligado;
 *  2. ligue o dispositivo para habilitar o modo de calibragem, medição inicial do diâmetro, crescimento e catalogo de dados;
 *  3. o modo de calibragem do sensor verifica se o carretel realizou no mímino uma volta completa;
 *  4. o modo de medição do DAP e ICAd captura por 20 segundos o valor inicial do diâmetro;
 *  5. o modo de crescimento coleta, a cada minuto, o valor do diâmetro (DAP) e compara com o último valor do DAP para registrar o incremento do diâmetro (ICAd);
 *  6. o modo de catalogo armazena, a cada minuto, um log em CSV no SDCard com os valores do DAP: inicial, final e o ICAd. 
 *  7. Para salvar o log, basta voltar o carretel para a posição inicial e aguardar o led vermelho acender. Depois disso, desligue o dispositivo e remova o SDCard.
*/
