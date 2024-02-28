package java_impl;

import java.net.ServerSocket;
import java.net.Socket;
import java.io.IOException;
import java.net.InetAddress;

// Server template under TCP protocol

public class Server {
    private static final String SERVER_NAME = "localhost";
    private static final short SERVER_PORT = 1234;
    private static final int BACKLOG = 5;

    public static void main(String[] args) {
        try(ServerSocket serverSocket = new ServerSocket(SERVER_PORT, BACKLOG, InetAddress.getByName(SERVER_NAME))) {
            // alternatively, we can bind here to a given address and a given port
            /* 
                InetSocketAddress socketAddress = new InetSocketAddress(SERVER_NAME, SERVER_PORT);
                serverSocket.bind(socketAddress, 5); // listen
            */


            while(true) {
                // accepting a client
                try(Socket client = serverSocket.accept()) {
                    // reading data from the client
                    int num1 = client.getInputStream().read();
                    assert num1 == 1;
                    int num2 = client.getInputStream().read();
                    assert num2 == 2;

                    // sending data to the client
                    client.getOutputStream().write(num1 + num2);
                    System.out.println("server done");
                } catch (IOException iOE) {
                    System.err.println(iOE.getMessage());
                }
            }
            
            
        } catch (IOException iOE) {
            System.err.println(iOE.getMessage());
        }
    }
}