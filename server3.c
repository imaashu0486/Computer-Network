#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8080

const char *html_response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Connection: close\r\n"
    "\r\n"
    "<!DOCTYPE html>"
    "<html lang=\"en\">"
    "<head>"
    "  <meta charset=\"UTF-8\">"
    "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
    "  <title>Advanced Client-Server C Demo</title>"
    "  <style>"
    "    :root {"
    "      --bg-light: #f0f4f8;"
    "      --bg-dark: #1a1a2e;"
    "      --text-light: #222;"
    "      --text-dark: #e0e0e0;"
    "      --primary: #4ecca3;"
    "      --button-bg: #0f3460;"
    "      --button-hover: #16213e;"
    "    }"
    "    body {"
    "      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;"
    "      background-color: var(--bg-light);"
    "      color: var(--text-light);"
    "      display: flex;"
    "      flex-direction: column;"
    "      justify-content: center;"
    "      align-items: center;"
    "      height: 100vh;"
    "      margin: 0;"
    "      transition: background-color 0.5s ease, color 0.5s ease;"
    "    }"
    "    .dark-mode {"
    "      background-color: var(--bg-dark);"
    "      color: var(--text-dark);"
    "    }"
    "    h1 {"
    "      font-size: 3rem;"
    "      margin-bottom: 0.3em;"
    "      text-shadow: 1px 1px 2px rgba(0,0,0,0.1);"
    "    }"
    "    button, input {"
    "      font-size: 1.1rem;"
    "      padding: 12px 24px;"
    "      margin: 10px 5px;"
    "      border-radius: 8px;"
    "      border: none;"
    "      cursor: pointer;"
    "      transition: background-color 0.3s ease, color 0.3s ease;"
    "      outline: none;"
    "      box-shadow: 0 5px 15px rgba(78, 204, 163, 0.4);"
    "    }"
    "    button {"
    "      background-color: var(--button-bg);"
    "      color: white;"
    "    }"
    "    button:hover {"
    "      background-color: var(--button-hover);"
    "    }"
    "    input {"
    "      border: 2px solid var(--primary);"
    "      color: var(--text-light);"
    "      width: 250px;"
    "      box-shadow: 0 5px 15px rgba(78, 204, 163, 0.2);"
    "    }"
    "    .dark-mode input {"
    "      background-color: #121825;"
    "      color: var(--text-dark);"
    "      border-color: var(--primary);"
    "    }"
    "    #output {"
    "      margin-top: 25px;"
    "      font-size: 1.4rem;"
    "      font-weight: 600;"
    "      min-height: 2em;"
    "    }"
    "    .footer {"
    "      position: absolute;"
    "      bottom: 10px;"
    "      font-size: 0.9rem;"
    "      color: #666;"
    "    }"
    "  </style>"
    "</head>"
    "<body>"
    "  <h1>Client-Server Model in C</h1>"
    "  <input id=\"nameInput\" type=\"text\" placeholder=\"Enter your name...\"/>"
    "  <button onclick=\"sayHello()\">Say Hello</button>"
    "  <button onclick=\"toggleDarkMode()\">Toggle Dark Mode</button>"
    "  <div id=\"output\"></div>"
    "  <div class=\"footer\">Made with ❤️ by Ashish Ranjan</div>"
    "  <script>"
    "    function sayHello() {"
    "      const name = document.getElementById('nameInput').value.trim();"
    "      const output = document.getElementById('output');"
    "      if(name.length === 0) {"
    "        output.innerText = 'Please enter your name!';"
    "        output.style.color = 'red';"
    "      } else {"
    "        output.style.color = 'var(--primary)';"
    "        output.innerText = `Hello, ${name}! 👋`;"
    "      }"
    "    }"
    "    function toggleDarkMode() {"
    "      document.body.classList.toggle('dark-mode');"
    "    }"
    "  </script>"
    "</body>"
    "</html>";

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[3000] = {0};

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Start listening
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("🚀 Server running at http://localhost:%d\n", PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        read(client_fd, buffer, sizeof(buffer) - 1);
        printf("📩 Request received:\n%s\n", buffer);

        // Send response
        write(client_fd, html_response, strlen(html_response));

        // Close connection
        close(client_fd);
    }

    return 0;
}

