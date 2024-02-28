package tema1.tema1a_6;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.*;
import java.util.ArrayList;

public class Server {
    private static final int SERVER_PORT = 1234;

    public static void main(String[] args) throws IOException {
        ServerSocket serverSocket = new ServerSocket(SERVER_PORT, 5);
        Socket socket = null;
        try {
            socket = serverSocket.accept();
        } catch (IOException iOE) {
            System.err.println(iOE.getMessage());
        }

        DataInputStream socketIn = new DataInputStream(socket.getInputStream());
        ArrayList<Integer> result = new ArrayList<>();

        char character = socketIn.readChar();
        System.out.println("[SERVER] Character received: " + character);
        
        int size = socketIn.readInt();
        System.out.println("[SERVER] Size received: " + size);

        char[] chars = new char[size];
        for(int i = 0; i < size; i++) {
            char ch = socketIn.readChar();
            chars[i] = ch;
        }

        System.out.println("[SERVER] Chars received: " + String.copyValueOf(chars));

        for(int i = 0; i < size; i++) {
            if(chars[i] == character) {
                result.add(i);
            }
        }

        DataOutputStream socketOut = new DataOutputStream(socket.getOutputStream());
        socketOut.write(result.size());
        
        for(Integer item : result) {
            try {
                socketOut.writeInt(item);
            } catch (IOException err) {
                System.err.println(err.getMessage());
            }
        }

        close(socket);
        close(serverSocket);
    }
}
