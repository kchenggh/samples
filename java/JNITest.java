//  Disclaimers
//  NO WARRANTY FOR SOURCE CODE
//  ALL SOURCE CODE ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED.
public class JNITest {
    public native String nativeHelloFromC();    

    static {
        System.loadLibrary("HelloFromC");
    }        

    public void print () {
		String str = nativeHelloFromC();
		System.out.println(str);
    }
    
    public static void main(String[] args) {
	    (new JNITest()).print();
		return;
    }
}
