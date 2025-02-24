#Análise de Vulnerabilidades em Redes Wi-Fi: Implementação de Fake AP com Captive Portal num Dispositivo IoT (ESP32)


Resumo — Este artigo aborda vulnerabilidades em dispositivos IoT exploradas através de redes fraudulentas, especificamente utilizando um Fake Access Point (Fake AP) combinado com um Captive Portal. O trabalho realizado inclui a implementação de um Fake AP baseado no ESP32 e a criação de um Captive Portal programado em C++ no Arduino IDE. Os resultados obtidos destacam a facilidade de exploração de dados sensíveis, como e-mails e palavras-passe, e apresentam recomendações para mitigar esses riscos. O artigo também explora os impactos deste tipo de ataque em locais públicos e a relevância da infraestrutura de rede, como Wi-Fi de 2.4 GHz e protocolos IPv4, no contexto de um Fake AP.
Palavras Chave – IoT, Fake AP, Captive Portal, IPv4.
Abstract — This article addresses vulnerabilities in IoT devices exploited through fraudulent networks, specifically using a Fake Access Point (Fake AP) combined with a Captive Portal. The work carried out includes the implementation of a Fake AP based on the ESP32 and the creation of a Captive Portal programmed in C on the Arduino IDE. The results obtained highlight the ease with which sensitive data, such as emails and passwords, can be exploited and present recommendations for mitigating these risks. The article also explores the impacts of this type of attack in public places and the relevance of network infrastructure, such as 2.4 GHz Wi-Fi and IPv4 protocols, in the context of a Fake AP.
Keywords – IoT, Fake AP, Captive Portal, IPv4.
I.	INTRODUÇÃO
A Internet das Coisas (IoT) [1] tem transformado diversos setores, permitindo a interconexão de dispositivos e a automação de processos. No entanto, o crescimento exponencial de dispositivos conectados também ampliou a superfície de ataque, expondo vulnerabilidades significativas. Entre os riscos mais comuns estão ataques baseados em redes Wi-Fi maliciosas, como Fake AP’s [2], que exploram a confiança dos utilizadores para recolher informações sensíveis.

Em ambientes públicos, como cafés, aeroportos ou bibliotecas, o impacto de ataques deste tipo é agravado pela alta concentração de utilizadores que muitas vezes conectam os seus dispositivos a redes abertas sem verificar a sua autenticidade. Essa vulnerabilidade torna-se ainda mais preocupante quando associada à falta de encriptação em redes Wi-Fi de 2.4 GHz, amplamente utilizadas devido à sua compatibilidade e maior alcance.

Neste contexto, o presente trabalho propõe uma abordagem prática para analisar essas vulnerabilidades através da implementação de um Fake AP e Captive Portal [3], destacando as implicações de segurança para dispositivos IoT e apresentando recomendações para mitigar os riscos associados.

II.	COMPARAÇÃO: FAKE AP VS. EVIL TWIN
Embora ambos os ataques tenham como objetivo enganar utilizadores através de redes fraudulentas, existem diferenças técnicas entre Fake AP e Evil Twin [4]:

Table 1 - Diferenças Principais do Fake AP com Evil Twin
Critério	Fake AP	Evil Twin
Configuração 	Criado como uma rede aberta ou com nome semelhante a redes legítimas.	Réplica exata de uma rede legítima, incluindo SSID e segurança (WPA/WPA2).
Autenticação	Geralmente não exige autenticação para atrair mais utilizadores.	Pode exigir autenticação para parecer mais convincente.
Complexidade	Simples de configurar, sem necessidade de ferramentas avançadas.	Requer ferramentas para clonar redes legítimas, como Airbase-ng [5]

Uso de Certificados	Não utiliza certificados de segurança.	Pode incluir certificados falsos para capturar credenciais em redes encriptadas.
Alvo Principal	Depende do utilizador conectar-se voluntariamente.	Utilizadores conectados a redes legítimas que são desconectados e redirecionados.


III.	METOLOGIA
As ferramentas utilizadas durante a implementação do Fake AP e o Captive Portal incluíram
•	ESP32 [6], um microcontrolador responsável pela criação do Fake AP, permitindo a simulação de uma rede Wi-Fi com configurações personalizadas.
•	Arduino IDE [7], um ambiente de desenvolvimento integrado, utilizado para programar o Captive Portal na linguagem C.

Enquanto trabalhos como “FakeAP Detector” [8] exploram soluções baseadas em aplicações para a deteção de hotspots maliciosos, este estudo adota uma abordagem prática para demonstrar, na perspetiva do atacante, as vulnerabilidades que podem ser exploradas em redes Wi-Fi abertas. A implementação de um Fake AP e Captive Portal com um ESP32 permite simular cenários reais de ataque, expondo como as credenciais dos utilizadores podem ser capturadas de forma simples.

A.	Funcionamento de um Fake AP
Um Fake AP é uma rede Wi-Fi maliciosa criada para imitar redes lícitas, geralmente configurada para atrair utilizadores desatentos. No caso deste trabalho, é configurado no ESP32 para transmitir um SSID atrativo. Redes de 2.4 GHz foram escolhidas por serem mais comuns e oferecerem maior alcance, o que aumenta a probabilidade de conexão de utilizadores. Além disso, essas redes utilizam protocolos baseados em IPv4 [9] para atribuir endereços IP aos dispositivos conectados. 

No pseudocódigo abaixo, é apresentada a configuração inicial do Fake AP. O ESP32 é configurado como ponto de acesso utilizando uma função para criar a rede, e outra para definir o endereço IP do dispositivo

Algorithm 1: Configuração do Fake AP
1: Definir o endereço IP estático do ponto de acesso para 192.168.4.1
2: Ativar o ponto de acesso
3: if pontoDeAcessoAtivo then
4:    Exibir "Ponto de acesso criado com sucesso!" no console serial
5:    Exibir o endereço IP configurado no console serial
6:else
7:     return "Erro na configuração do Fake AP"
8: end if


Uma vez conectado, o utilizador recebe um endereço IP dinâmico pelo protocolo DHCP [10]. Posteriormente, ao tentar aceder a uma página web, o utilizador é redirecionado para um Captive Portal.




B.	Desenvolvimento do Captive Portal
O Captive Portal é uma página web apresentada automaticamente aos utilizadores conectados ao Fake AP. A implementação baseou-se em dois componentes principais: o DNSServer [11] e o WebServer [12], configurados para garantir que todos os pedidos DNS dos utilizadores fossem redirecionados para o ESP32. Esse redirecionamento foi realizado de forma a capturar todas as consultas DNS e direcioná-las para o endereço IP do ponto de acesso configurado (192.168.4.1).

Por outro lado, o WebServer foi utilizado para apresentar a interface do Captive Portal, onde os utilizadores inserem as suas credenciais. A página de login foi projetada para simular uma interface autêntica de acesso à internet, solicitando o e-mail e a palavra-passe do utilizador. Após a submissão das credenciais, o servidor armazenava localmente os dados para análise posterior.

Além disso, uma estratégia adicional foi implementada para aumentar a probabilidade de obtenção de múltiplas credenciais do mesmo utilizador. Sempre que o utilizador inseria os seus dados, uma mensagem de "erro de credenciais" era exibida na página, induzindo-o a acreditar que o e-mail estava associado a uma palavra-passe incorreta. Essa abordagem incentivava o utilizador a tentar novas combinações de senhas, permitindo capturar diversas tentativas de credenciais associadas ao mesmo endereço de e-mail.

Captive Portals maliciosos, como o implementado neste trabalho, têm o potencial de explorar a interação dos utilizadores com redes Wi-Fi abertas. Estudos como o "Browser History Stealing with Captive Wi-Fi Portals" [13] demonstram como estes mecanismos podem ser utilizados para extraír dados sensíveis, ampliando os riscos enfrentados pelos utilizadores em redes desprotegidas.

O pseudocódigo abaixo apresenta a implementação do Captive Portal, incluindo o redirecionamento DNS, a exibição da página de login e a captura das credenciais submetidas pelos utilizadores:


Algoritmo 2: Desenvolvimento do Captive Portal
1: Definir o endereço IP estático do ponto de acesso para 192.168.4.1
2: Iniciar o servidor DNS para redirecionar todas as consultas DNS para o ponto de acesso
3: Iniciar o servidor web para gerir requisições HTTP
4: Configurar o URL para apresentar a página de login
5: Configurar URL para capturar credenciais submetidas pelos utilizadores
6: Configurar redirecionamento de URLs desconhecidas para a página principal
7: Ativar os servidores DNS e web
8: Loop:
8.1: Processar pedidos DNS
8.2: Processar pedidos HTTP
9: if credenciais forem submetidas pelo utilizador then
10:      Armazenar e-mail e palavra-passe
11:      Exibir mensagem de "Erro de credenciais" para incentivar novas tentativas
12: else
13:      Continuar a processar requisições
14: end if

C.	Rescolha de dados
A recolha de dados no Captive Portal foi implementada de forma a capturar as credenciais submetidas pelos utilizadores que se conectavam ao Fake AP. Quando os utilizadores tentavam autenticar-se na página de login simulada, os dados inseridos (como o e-mail e a palavra-passe) eram enviados para o servidor HTTP do ESP32.

A função responsável por processar essas submissões foi configurada para capturar os parâmetros username e password enviados via requisições HTTP POST. Após a recolha, os dados eram exibidos no console serial para análise em tempo real, simulando a extração de credenciais de um utilizador desavisado.

O código abaixo demonstra o mecanismo de recolha de dados implementado:

Algorithm 3: Recolha de Dados
1: Receber a submissão de dados do utilizador via HTTP POST
2: if os parâmetros "email" e "password" estiverem presentes then
3:     Capturar o valor de "email"
4:     Capturar o valor de "password"
5:     Exibir "Credenciais capturadas" na consola serial
6:     Exibir "Email" e "Palavra-Passe" capturados na consola serial
7:     Enviar uma mensagem de "Erro: Credenciais inválidas" para a página de login, incentivando novas tentativas
8: else
9:     Enviar mensagem de "Erro: Dados incompletos" para a página de login
10: end if


IV.	RESULTADO E DISCUSSÃO

Durante a execução do experimento, foi possível capturar as credenciais inseridas pelos utilizadores que se conectaram ao Fake AP. As credenciais capturadas foram exibidas em tempo real no console serial do ESP32, simulando o impacto que um ataque real pode ter. Apesar de não expor informações sensíveis neste artigo, o experimento demonstrou a vulnerabilidade dos utilizadores ao se conectarem a redes Wi-Fi abertas sem verificar sua legitimidade.

Estes resultados são consistentes como "On Privacy Risks of Public WiFi Captive Portals" [14], que demonstraram que muitos portais cativos em redes públicas não apenas capturam informações sensíveis, mas também rastreiam as atividades dos utilizadores. Este comportamento compromete a privacidade e segurança dos dados, ampliando o impacto dos ataques maliciosos. A presente implementação evidencia que esses portais podem ser facilmente explorados para a captura de credenciais, reforçando a necessidade de mecanismos de segurança mais robustos para proteger os utilizadores.

A.	Potenciais Impactos do Ataque em Dispositivos IoT
O projeto demonstrado no experimento expõe uma preocupação significativa em relação a dispositivos IoT, que frequentemente operam em redes abertas ou inseguras. Dispositivos IoT com autenticação fraca ou configurados para conectar-se automaticamente a redes conhecidas podem ser comprometidos ao se conectarem a redes maliciosas. O impacto inclui:

•	Roubo de dados sensíveis enviados por dispositivos IoT.
•	Injeção de comandos maliciosos para alterar o funcionamento de dispositivos.
•	Potencial inclusão desses dispositivos em redes de botnets [15].

V.	SOLUÇÕES E RECOMENDAÇÕES
•	Evitar manter o Wi-Fi sempre ligado em locais públicos.
•	Verificar o nome da rede (SSID) antes de conectar-se a qualquer rede. Estudos como “Privacy Preserving Open-Access 802.11 Public Hotspot AP Authentication Mechanism” [16] propõem mecanismos de autenticação que minimizam os riscos associados ao uso de redes Wi-Fi abertas, abordando as fragilidades exploradas por ataques como Fake APs."
•	Evitar inserção de credenciais sensíveis em páginas de login apresentadas após conectar-se a redes abertas


VI.	IMPACTO LEGAL E ÉTICO DOS FAKE APS E CAPTIVE PORTALS

A implementação de Fake APs e Captive Portals levanta questões éticas e legais relevantes no contexto da cibersegurança. A captura de credenciais e informações pessoais sem consentimento pode infringir diversas regulamentações de proteção de dados, como o Regulamento Geral sobre a Proteção de Dados (RGPD) da União Europeia, especificamente o Artigo 6º (Licitude do tratamento) e o Artigo 32º (Segurança do tratamento) [17], que impõem requisitos rigorosos sobre a coleta, processamento e armazenamento de informações pessoais.

A criação e operação de Fake APs podem ser interpretadas como uma violação de leis de cibersegurança em vários países. No caso da União Europeia, a Diretiva NIS (Network and Information Security), em particular o Artigo 14º (Requisitos de segurança para operadores de serviços essenciais) [18], estabelece penalidades severas para práticas que envolvam a coleta indevida de dados pessoais. Em Portugal, o Regulamento da Proteção de Dados e a Lei do Cibercrime (Lei n.º 109/2009) [19] também prevêem sanções para atividades que comprometam a privacidade e a segurança dos utilizadores.

Além disso, a responsabilidade dos operadores de redes públicas deve ser considerada. Hotspots Wi-Fi disponibilizados em locais públicos, como cafés e aeroportos, frequentemente não implementam medidas de segurança adequadas para proteger os seus utilizadores. De acordo com o RGPD, empresas e instituições que oferecem acesso público à Internet podem ser responsabilizadas, segundo o Artigo 24º (Responsabilidade do responsável pelo tratamento) [20], caso não tomem as precauções necessárias para garantir a segurança da conexão, prevenindo a ação de atacantes que criam redes falsas para enganar os utilizadores.

Do ponto de vista ético, a exploração dessas vulnerabilidades para fins de pesquisa deve ser conduzida em ambientes controlados e sem prejudicar utilizadores reais. Trabalhos como este são essenciais para demonstrar riscos e incentivar a adoção de melhores práticas de segurança, mas devem sempre seguir normas de ética na pesquisa e respeitar a privacidade e a integridade das informações.



VII.	CONCLUSÕES
A utilização crescente de dispositivos IoT, como o ESP32, oferece inúmeras oportunidades para inovações tecnológicas, mas também levanta preocupações significativas no que diz respeito à segurança. Este trabalho demonstrou como um ESP32, um dispositivo acessível e de fácil utilização, pode ser aproveitado para implementar um Fake AP e um Captive Portal, simulando um cenário que atrai utilizadores com smartphones e recolhe as informações introduzidas. Este projeto foi conduzido num ambiente privado, controlado e ético, onde apenas foram utilizados dados fictícios e sem ligação a informações reais. Apesar disso, o estudo sublinha claramente os riscos associados à utilização imprudente de redes Wi-Fi abertas.
O principal objetivo deste trabalho foi salientar a necessidade de os utilizadores verificarem a legitimidade das redes Wi-Fi antes de estabelecerem ligação, especialmente quando lhes são solicitados dados sensíveis, como endereços de e-mail ou palavras-passe. Redes maliciosas, configuradas como Fake APs em conjunto com Captive Portals que imitam autenticidade, conseguem enganar facilmente os utilizadores, expondo informações pessoais ou empresariais a potenciais riscos.
Os resultados obtidos neste estudo revelam o impacto significativo que este tipo de ataque pode ter, particularmente em contextos reais, onde os utilizadores tendem a ligar-se a redes públicas sem tomar precauções adequadas. Estas situações podem ser evitadas com medidas simples, como desativar as conexões automáticas, evitar redes desconhecidas e desconfiar de páginas de login inesperadas.
Por outro lado, a simplicidade do hardware e do software utilizado neste projeto realça a necessidade de os fabricantes de dispositivos IoT adoptarem medidas de segurança mais rigorosas por defeito. Estas devem incluir a autenticação obrigatória, a encriptação de dados e restrições que impeçam conexões automáticas a redes não verificadas. Trabalhos como este não só expõem vulnerabilidades existentes, mas também servem como um alerta para a indústria tecnológica e para a comunidade académica, reforçando a urgência de avanços contínuos em cibersegurança.


Referências Bibliográficas

[1] 	"Internet of Things (IoT), O que é e porque é importante?," SAS, [Online]. Available: https://www.sas.com/pt_pt/insights/big-data/internet-of-things.html. [Accessed 20 Janeiro 2025].
[2] 	"What is a Fake AP? How It Works & Examples," Twingate Team, 1 Agosto 2024. [Online]. Available: https://www.twingate.com/blog/glossary/fake%20ap. [Accessed 20 Janeiro 2025].
[3] 	"Captive portals," Fortinet, [Online]. Available: https://docs.fortinet.com/document/fortigate/7.6.1/administration-guide/934626/captive-portals. [Accessed 20 Janeiro 2025].
[4] 	"Evil Twin Attack: Fake WiFi Access Point Vulnerabilities," okta, 29 Agosto 2024. [Online]. Available: https://www.okta.com/identity-101/evil-twin-attack/. [Accessed 20 Janeiro 2025].
[5] 	"Description," Airbase-ng.org, [Online]. Available: https://www.aircrack-ng.org/doku.php?id=airbase-ng. [Accessed 20 Janeiro 2025].
[6] 	"ESP32 for IoT: A Complete Guide," Nabto, [Online]. Available: https://www.nabto.com/guide-to-iot-esp-32/. [Accessed 20 Janeiro 2025].
[7] 	"Arduino Integrated Development Environment (IDE) v1," Arduino.cc, 17 Janeiro 2024. [Online]. Available: https://docs.arduino.cc/software/ide-v1/tutorials/arduino-ide-v1-basics/. [Accessed 20 Janeiro 2025].
[8] 	M. LUNODZO J., A. ABEL Z., K. SHUBI F. and N. JEMA D, " FakeAP Detector: An Android-Based Client-Side," in IEEE, 2022. 
[9] 	"Classes IPv4: O que são e como são usados," Hostwinds Teams, 23 Maio 2024. [Online]. Available: https://www.hostwinds.pt/blog/ipv4-classes-what-are-they-and-how-are-they-used. [Accessed 25 Janeiro 2025].
[10] 	"O que é o Protocolo de configuração dinâmica de host (DHCP)?," Fortinet, [Online]. Available: https://www.fortinet.com/br/resources/cyberglossary/dynamic-host-configuration-protocol-dhcp. [Accessed 20 Janeiro 2025].
[11] 	"O que é um servidor de DNS?," Cloudflare, [Online]. Available: https://www.cloudflare.com/pt-br/learning/dns/what-is-a-dns-server/. [Accessed 20 Janeiro 2025].
[12] 	"Web Server: O que é e Como Funciona?," Hostinger, 20 Abril 2023. [Online]. Available: https://www.hostinger.com.br/tutoriais/web-server. [Accessed 20 Janeiro 2025].
[13] 	A. Dabrowski, G. Merzdovnik, N. Kommenda and E. Weippl, "Browser History Stealing," in Competence Centers for Excellent Technologies, 2016. 
[14] 	S. Ali, T. Osman, M. Mannan and A. Youssef, "On Privacy Risks of Public WiFi Captive Portals," Cornell University, 2019. 
[15] 	"O que é botnet?," kaspersky, [Online]. Available: https://www.kaspersky.com.br/resource-center/threats/botnet-attacks. [Accessed 23 Janeiro 2025].
[16] 	A. Srinivasan and J. Wu, "VOUCH-AP: privacy preserving open-access 802.11 public hotspot AP authentication mechanism with co-located evil-twins," in International Journal od Security and Networks, 2018. 
[17] 	"REGULAMENTO (UE) 2016/679 DO PARLAMENTO EUROPEU E DO CONSELHO," RGPD, 27 Abril 2016. [Online]. Available: https://eur-lex.europa.eu/legal-content/PT/TXT/PDF/?uri=CELEX:32016R0679. [Accessed 5 Fevereiro 2025].
[18] 	"DIRETIVA (UE) 2016/1148 DO PARLAMENTO EUROPEU E DO CONSELHO," CNCS (Centro Nacional de Cibersegurança), 6 Julho 2016. [Online]. Available: https://www.cncs.gov.pt/docs/diretiva-2016.pdf. [Accessed 5 Fevereiro 2025].
[19] 	"Lei n.º 109/2009, de 15 de setembro," Diário Da República, 15 Setembro 2009. [Online]. Available: https://diariodarepublica.pt/dr/detalhe/lei/109-2009-489693. [Accessed 5 Fevereiro 2025].
[20] 	"Regulamento(UE) n.º 679/2016, de 27 de Abril," Ministério Público Portugal, 27 Abril 2016. [Online]. Available: https://www.pgdlisboa.pt/leis/lei_mostra_articulado.php?nid=2961&tabela=leis. [Accessed 5 Fevereiro 2025].


