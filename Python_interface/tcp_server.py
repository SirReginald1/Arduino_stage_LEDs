from typing import Optional
import socket
import threading
import queue
import time


class ThreadedTCPServer:
    """This class is used to start a server to enable LED control by sending commands to this server."""

    def __init__(self,
                 app: "App",
                 host: str = '127.0.0.1',
                 port: str = 80) -> "ThreadedTCPServer":
        """Constructor for TCP server.
        
        ### Args:
            - app (App): The aplication runing the server.
            - host (str, Optional): The IP address of the server.
            - port (str, Optional): The port used by the server.
        """
        self.app = app
        self.host: str = host
        self.port: int = port
        self.running: bool = False
        self.server_thread: Optional[threading.Thread] = None
        self.server_socket: Optional[socket.socket] = None
        self.message_queue = queue.Queue()
        self.client_threads: list[threading.Thread] = []
        self.client_sockets: list[socket.socket] = []

    def set_address_and_port(self,
                             host: str,
                             port: int) -> int:
        """Function used to to change the host and port of the server.
        Can only change the host and port if the server is not running.

        ### Args:
            - host (str): The IP address at which the server will be hosted.
            - port (int): The port on which the server will be listening.
        
        ### Returns:
            - int: 0 if port and address are changed successfully else 1.
        """
        if not self.running:
            self.host = host
            self.port = port
            return 0
        return 1

    def start(self) -> None:
        """Function used to start up the server."""
        self.running = True
        self.server_thread = threading.Thread(target=self.run_client_handeler,
                                              daemon=True)
        self.server_thread.start()
        self.message_sender_thread = threading.Thread(target=self.run_message_sending,
                                                      daemon=True)
        self.message_sender_thread.start()

    def run_client_handeler(self) -> None:
        """Main loop for runing the server."""
        with socket.socket(socket.AF_INET,
                           socket.SOCK_STREAM) as s:
            self.server_socket = s
            s.setsockopt(socket.SOL_SOCKET,
                         socket.SO_REUSEADDR, 1)
            s.bind((self.host, self.port))
            s.listen()
            print(f"[SERVER] Listening on {self.host}:{self.port}...")
            s.settimeout(1.0)

            while self.running:
                try:
                    conn, addr = s.accept()
                    print(f"[SERVER] Accepted connection from {addr}")
                    client_thread = threading.Thread(target=self.handle_client,
                                                     args=(conn,),
                                                     daemon=True)
                    client_thread.start()
                    self.client_threads.append(client_thread)
                    self.client_sockets.append(conn)
                except socket.timeout:
                    continue
    
    def run_message_sending(self) -> None:
        """Thread the handles sending messages to the controller."""
        while self.running:
            if not self.message_queue.empty():
                message = self.message_queue.get()
                self.app.controller_interface.send_message(message)
                #print(f"[SERVER] {message}")
            time.sleep(0.0001)
    
    def handle_client(self, conn: socket.socket) -> None:
        with conn:
            #conn.settimeout(1.0)
            while self.running:
                try:
                    data = conn.recv(1024)
                    if not data:
                        break
                    message = data.decode().strip()
                    print(message)
                    self.message_queue.put(message)
                except socket.timeout:
                    continue
                except ConnectionResetError:
                    break
            print("[SERVER] Client disconnected.")

    def stop(self) -> None:
        """Function that stop the server."""
        print("[SERVER] Stopping server...")
        self.running = False

        for sock in self.client_sockets:
            try:
                sock.shutdown(socket.SHUT_RDWR)
                sock.close()
            except:
                pass

        for thread in self.client_threads:
            thread.join(timeout=1)

        if self.server_thread:
            self.server_thread.join(timeout=2)

        print("[SERVER] Server stopped.")

    def get_messages(self) -> list[str]:
        """Function used to get messages received by the server.
        
        ### Returns:
            - list: List of messages in the form of strings.
        """
        messages = []
        while not self.message_queue.empty():
            messages.append(self.message_queue.get())
        return messages

# ----------------------------------------
# Main application logic

#if __name__ == "__main__":
#    server = ThreadedTCPServer()
#    server.start()
#
#    try:
#        print("[APP] Main loop started. Waiting for messages...")
#        for _ in range(30):  # Run for 30 seconds (or loop count)
#            messages = server.get_messages()
#            for msg in messages:
#                print(msg)
#            time.sleep(1)
#    finally:
#        server.stop()
#