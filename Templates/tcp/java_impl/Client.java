package java_impl;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetSocketAddress;
import java.net.Socket;

// Client Template under TCP protocol

public class Client {
    private static final String SERVER_NAME = "localhost";
    private static final short SERVER_PORT = 1234;

    public static void main(String[] args) {
        try(Socket client = new Socket()) {
            // initializing the server socket
            InetSocketAddress sAddress = new InetSocketAddress(SERVER_NAME, SERVER_PORT);
            try(BufferedReader reader = new BufferedReader(new InputStreamReader(System.in))) {
                client.connect(sAddress); // connecting to the server

                // similar to the pipe mechanism

                // writing in output
                client.getOutputStream().write(1);
                client.getOutputStream().write(2);
                // reading from input
                int result = client.getInputStream().read();
                assert result == 3;
                System.out.println("client done");

            } catch (IOException iOE) {
                System.err.println(iOE.getMessage());
            }
        } catch (IOException iOE) {
            System.err.println(iOE.getMessage());
        }
    }

}
