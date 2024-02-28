import java.net.*;
import java.nio.charset.StandardCharsets;
import java.io.*;

public class Client {
    private static final String SERVER_ADDR = "127.0.0.1";
    private static final int SERVER_PORT = 1234;

    private static void closeStreams(Socket socket, BufferedReader bufferedReader) {
        if (socket != null) {
            try {
                socket.close();
                System.out.println("Closed socket!");
            } catch (IOException iOE) {
                System.err.println("Couldn't close socket! " + iOE.getMessage());
            }
        }

        if (bufferedReader != null) {
            try {
                bufferedReader.close();
                System.out.println("Closed reader!");
            } catch (IOException iOE) {
                System.err.println("Couldn't close reader! " + iOE.getMessage());
            }
        }
    }

    private static String readString(BufferedReader reader) throws IOException {
        System.out.print("[CLIENT] Read string: ");
        
        try {
            String string = reader.readLine();
            return string;
        } catch (IOException iOE) {
            System.err.println("[CLIENT] Couldn't read string!" + iOE.getMessage());
        }
        return null;
    }

    private static void writeStringToSocket(String string, Socket socket) throws IOException {
        DataOutputStream socketOut = new DataOutputStream(socket.getOutputStream());

        short stringLength = (short) string.length();
        byte[] stringBytes = string.getBytes(StandardCharsets.US_ASCII);

        socketOut.writeShort(stringLength);
        
        try {
            socketOut.write(stringBytes);
            socketOut.flush();
        } catch (IOException iOE) {
            System.err.println("[CLIENT] Couldn't send string! " + iOE.getMessage());
        }
    }

    private static String readStringFromSocket(Socket socket) throws IOException{
        DataInputStream socketIn = new DataInputStream(socket.getInputStream());

        try {
            short stringLength = socketIn.readShort();
            byte[] stringBytes = new byte[stringLength];
            socketIn.readFully(stringBytes);
            return new String(stringBytes, StandardCharsets.US_ASCII);
        } catch (IOException iOE) {
            System.err.println("[CLIENT] Couldn't read string! " + iOE.getMessage());
        }

        return null;
    }

    public static void main(String[] args) throws Exception {
        Socket client = null;
        BufferedReader reader = null;
        try {
            reader = new BufferedReader(new InputStreamReader(System.in));
            client = new Socket(SERVER_ADDR, SERVER_PORT);

            String string = readString(reader);
            if (string == null) {
                return;
            }
            
            writeStringToSocket(string, client);
            String result = readStringFromSocket(client);
            
            if(result != null) {
                System.out.println("[CLIENT] Received from server: " + result);
            }
        } catch (IOException iOE) {
            System.err.println("[CLIENT] Caught exception: " + iOE.getMessage());
        } finally {
            closeStreams(client, reader);
        }
    }
}
