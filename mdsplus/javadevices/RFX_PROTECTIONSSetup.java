/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author manduchi
 */
public class RFX_PROTECTIONSSetup extends DeviceSetup {

    /**
     * Creates new form RFX_PROTECTIONSSetup
     */
    public RFX_PROTECTIONSSetup() {
        initComponents();
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        deviceButtons1 = new DeviceButtons();
        jPanel1 = new javax.swing.JPanel();
        jPanel3 = new javax.swing.JPanel();
        deviceField1 = new DeviceField();
        deviceField2 = new DeviceField();
        deviceField3 = new DeviceField();
        deviceField4 = new DeviceField();
        deviceField5 = new DeviceField();
        deviceField6 = new DeviceField();
        jPanel2 = new javax.swing.JPanel();
        jPanel6 = new javax.swing.JPanel();
        jPanel7 = new javax.swing.JPanel();
        deviceField7 = new DeviceField();
        jPanel5 = new javax.swing.JPanel();
        deviceField8 = new DeviceField();
        jPanel4 = new javax.swing.JPanel();
        deviceField9 = new DeviceField();

        setDeviceProvider("localhost");
        setDeviceTitle("Protezioni di Macchina RFX");
        setDeviceType("RFX_PROTECTIONS");
        setHeight(500);
        setWidth(600);
        getContentPane().add(deviceButtons1, java.awt.BorderLayout.SOUTH);

        jPanel1.setLayout(new java.awt.GridLayout(3, 1));

        jPanel3.setBorder(javax.swing.BorderFactory.createTitledBorder("Protezioni Br"));
        jPanel3.setLayout(new java.awt.GridLayout(4, 2));

        deviceField1.setIdentifier("");
        deviceField1.setLabelString("Max Br Orizzontale (T) (Warning): ");
        deviceField1.setNumCols(4);
        deviceField1.setOffsetNid(2);
        jPanel3.add(deviceField1);

        deviceField2.setIdentifier("");
        deviceField2.setLabelString("Max Br Orizzontale (T) (Fault): ");
        deviceField2.setNumCols(4);
        deviceField2.setOffsetNid(3);
        jPanel3.add(deviceField2);

        deviceField3.setIdentifier("");
        deviceField3.setLabelString("Max Br verticale (T) (Warning): ");
        deviceField3.setNumCols(4);
        deviceField3.setOffsetNid(4);
        jPanel3.add(deviceField3);

        deviceField4.setIdentifier("");
        deviceField4.setLabelString("Max Br Verticale (T)(Fault): ");
        deviceField4.setNumCols(4);
        deviceField4.setOffsetNid(5);
        jPanel3.add(deviceField4);

        deviceField5.setIdentifier("");
        deviceField5.setLabelString("Max tempo fuori soglia (s)  (Warning):");
        deviceField5.setNumCols(4);
        deviceField5.setOffsetNid(6);
        jPanel3.add(deviceField5);

        deviceField6.setIdentifier("");
        deviceField6.setLabelString("Max tempo fuori soglia (s)(Fault):");
        deviceField6.setNumCols(4);
        deviceField6.setOffsetNid(7);
        jPanel3.add(deviceField6);

        jPanel1.add(jPanel3);

        jPanel2.setBorder(javax.swing.BorderFactory.createTitledBorder("Protezioni Bobine a Sella"));
        jPanel2.setLayout(new java.awt.BorderLayout());

        jPanel6.setLayout(new java.awt.GridLayout(2, 1));

        deviceField7.setIdentifier("");
        deviceField7.setLabelString("Max corrente Chopper Bobine a Sella(A): ");
        deviceField7.setNumCols(4);
        deviceField7.setOffsetNid(8);
        jPanel7.add(deviceField7);

        jPanel6.add(jPanel7);

        deviceField8.setIdentifier("");
        deviceField8.setLabelString("Limite I2T Bobine a Sella (A2s): ");
        deviceField8.setNumCols(4);
        deviceField8.setOffsetNid(9);
        jPanel5.add(deviceField8);

        jPanel6.add(jPanel5);

        jPanel2.add(jPanel6, java.awt.BorderLayout.CENTER);

        jPanel1.add(jPanel2);

        jPanel4.setBorder(javax.swing.BorderFactory.createTitledBorder("Protezioni Bobile Toroidali"));

        deviceField9.setIdentifier("");
        deviceField9.setLabelString("Limite I2T Bobine Toroidali (A2s): ");
        deviceField9.setNumCols(4);
        deviceField9.setOffsetNid(10);
        jPanel4.add(deviceField9);

        jPanel1.add(jPanel4);

        getContentPane().add(jPanel1, java.awt.BorderLayout.CENTER);

        getAccessibleContext().setAccessibleName("");
    }// </editor-fold>//GEN-END:initComponents
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private DeviceButtons deviceButtons1;
    private DeviceField deviceField1;
    private DeviceField deviceField2;
    private DeviceField deviceField3;
    private DeviceField deviceField4;
    private DeviceField deviceField5;
    private DeviceField deviceField6;
    private DeviceField deviceField7;
    private DeviceField deviceField8;
    private DeviceField deviceField9;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JPanel jPanel3;
    private javax.swing.JPanel jPanel4;
    private javax.swing.JPanel jPanel5;
    private javax.swing.JPanel jPanel6;
    private javax.swing.JPanel jPanel7;
    // End of variables declaration//GEN-END:variables
}
