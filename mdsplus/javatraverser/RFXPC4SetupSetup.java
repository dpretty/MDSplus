/*
 * RFXPCAT4SetupSetup.java
 *
 * Created on 16 aprile 2009, 8.48
 */



/**
 *
 * @author  manduchi
 */
public class RFXPC4SetupSetup extends DeviceSetup {

    /** Creates new form BeanForm */
    public RFXPC4SetupSetup() {
        initComponents();
    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        deviceButtons1 = new DeviceButtons();
        jPanel1 = new javax.swing.JPanel();
        jPanel2 = new javax.swing.JPanel();
        deviceField1 = new DeviceField();
        jPanel3 = new javax.swing.JPanel();
        deviceField2 = new DeviceField();
        deviceField3 = new DeviceField();
        jPanel4 = new javax.swing.JPanel();
        deviceField4 = new DeviceField();
        deviceChoice1 = new DeviceChoice();
        jTabbedPane1 = new javax.swing.JTabbedPane();
        deviceChannel1 = new DeviceChannel();
        jPanel5 = new javax.swing.JPanel();
        deviceField5 = new DeviceField();
        deviceField6 = new DeviceField();
        deviceField7 = new DeviceField();
        deviceField8 = new DeviceField();
        deviceField9 = new DeviceField();
        deviceWave1 = new DeviceWave();
        deviceChannel2 = new DeviceChannel();
        jPanel6 = new javax.swing.JPanel();
        deviceField10 = new DeviceField();
        deviceField11 = new DeviceField();
        deviceField12 = new DeviceField();
        deviceField13 = new DeviceField();
        deviceField14 = new DeviceField();
        deviceWave2 = new DeviceWave();
        deviceChannel3 = new DeviceChannel();
        jPanel7 = new javax.swing.JPanel();
        deviceField15 = new DeviceField();
        deviceField16 = new DeviceField();
        deviceField17 = new DeviceField();
        deviceField18 = new DeviceField();
        deviceField19 = new DeviceField();
        deviceWave3 = new DeviceWave();
        deviceChannel4 = new DeviceChannel();
        jPanel8 = new javax.swing.JPanel();
        deviceField20 = new DeviceField();
        deviceField21 = new DeviceField();
        deviceField22 = new DeviceField();
        deviceField23 = new DeviceField();
        deviceField24 = new DeviceField();
        deviceWave4 = new DeviceWave();

        setDeviceProvider("localhost");
        setDeviceTitle("PCAT Setup");
        setDeviceType("RFXPC4Setup");
        setHeight(500);
        setWidth(700);
        getContentPane().add(deviceButtons1, java.awt.BorderLayout.PAGE_END);

        jPanel1.setLayout(new java.awt.GridLayout(3, 1));

        deviceField1.setIdentifier("");
        deviceField1.setLabelString("Comment:");
        deviceField1.setNumCols(30);
        deviceField1.setOffsetNid(1);
        deviceField1.setTextOnly(true);
        jPanel2.add(deviceField1);

        jPanel1.add(jPanel2);

        deviceField2.setDisplayEvaluated(true);
        deviceField2.setEditable(false);
        deviceField2.setIdentifier("");
        deviceField2.setLabelString("Enabled Units: ");
        deviceField2.setNumCols(15);
        deviceField2.setOffsetNid(5);
        deviceField2.setTextOnly(true);
        jPanel3.add(deviceField2);

        deviceField3.setDisplayEvaluated(true);
        deviceField3.setEditable(false);
        deviceField3.setIdentifier("");
        deviceField3.setLabelString("Window");
        deviceField3.setNumCols(5);
        deviceField3.setOffsetNid(4);
        jPanel3.add(deviceField3);

        jPanel1.add(jPanel3);

        deviceField4.setDisplayEvaluated(true);
        deviceField4.setEditable(false);
        deviceField4.setIdentifier("");
        deviceField4.setLabelString("Connection");
        deviceField4.setOffsetNid(2);
        deviceField4.setTextOnly(true);
        jPanel4.add(deviceField4);

        deviceChoice1.setChoiceItems(new String[] {"CURRENT", "VOLTAGE", "OPEN LOOP"});
        deviceChoice1.setIdentifier("");
        deviceChoice1.setLabelString("Control:");
        deviceChoice1.setOffsetNid(3);
        deviceChoice1.setUpdateIdentifier("");
        jPanel4.add(deviceChoice1);

        jPanel1.add(jPanel4);

        getContentPane().add(jPanel1, java.awt.BorderLayout.PAGE_START);

        deviceChannel1.setLabelString("");
        deviceChannel1.setOffsetNid(6);
        deviceChannel1.setShowState(false);
        deviceChannel1.setShowVal("");
        deviceChannel1.setUpdateIdentifier("");
        deviceChannel1.getContainer().setLayout(new java.awt.BorderLayout());

        deviceField5.setIdentifier("");
        deviceField5.setLabelString("Pert. Ampl. (A):");
        deviceField5.setNumCols(4);
        deviceField5.setOffsetNid(13);
        jPanel5.add(deviceField5);

        deviceField6.setIdentifier("");
        deviceField6.setLabelString("Freq. (Hz):");
        deviceField6.setNumCols(4);
        deviceField6.setOffsetNid(14);
        jPanel5.add(deviceField6);

        deviceField7.setIdentifier("");
        deviceField7.setLabelString("Phase (Rad):");
        deviceField7.setNumCols(4);
        deviceField7.setOffsetNid(15);
        jPanel5.add(deviceField7);

        deviceField8.setIdentifier("");
        deviceField8.setLabelString("Start Time (s):");
        deviceField8.setNumCols(4);
        deviceField8.setOffsetNid(16);
        jPanel5.add(deviceField8);

        deviceField9.setIdentifier("");
        deviceField9.setLabelString("End Time (s):");
        deviceField9.setNumCols(4);
        deviceField9.setOffsetNid(17);
        jPanel5.add(deviceField9);

        deviceChannel1.getContainer().add(jPanel5, java.awt.BorderLayout.NORTH);

        deviceWave1.setIdentifier("");
        deviceWave1.setOffsetNid(7);
        deviceWave1.setUpdateExpression("");
        deviceChannel1.getContainer().add(deviceWave1, java.awt.BorderLayout.CENTER);

        jTabbedPane1.addTab("Wave 1", deviceChannel1);

        deviceChannel2.setLabelString("");
        deviceChannel2.setOffsetNid(18);
        deviceChannel2.setShowState(false);
        deviceChannel2.setShowVal("");
        deviceChannel2.setUpdateIdentifier("");
        deviceChannel2.getContainer().setLayout(new java.awt.BorderLayout());

        deviceField10.setIdentifier("");
        deviceField10.setLabelString("Pert. Ampl. (A):");
        deviceField10.setNumCols(4);
        deviceField10.setOffsetNid(25);
        jPanel6.add(deviceField10);

        deviceField11.setIdentifier("");
        deviceField11.setLabelString("Freq. (Hz):");
        deviceField11.setNumCols(4);
        deviceField11.setOffsetNid(26);
        jPanel6.add(deviceField11);

        deviceField12.setIdentifier("");
        deviceField12.setLabelString("Phase (Rad):");
        deviceField12.setNumCols(4);
        deviceField12.setOffsetNid(27);
        jPanel6.add(deviceField12);

        deviceField13.setIdentifier("");
        deviceField13.setLabelString("Start Time (s):");
        deviceField13.setNumCols(4);
        deviceField13.setOffsetNid(28);
        jPanel6.add(deviceField13);

        deviceField14.setIdentifier("");
        deviceField14.setLabelString("End Time (s):");
        deviceField14.setNumCols(4);
        deviceField14.setOffsetNid(29);
        jPanel6.add(deviceField14);

        deviceChannel2.getContainer().add(jPanel6, java.awt.BorderLayout.NORTH);

        deviceWave2.setIdentifier("");
        deviceWave2.setOffsetNid(19);
        deviceWave2.setUpdateExpression("");
        deviceChannel2.getContainer().add(deviceWave2, java.awt.BorderLayout.CENTER);

        jTabbedPane1.addTab("Wave 2", deviceChannel2);

        deviceChannel3.setLabelString("");
        deviceChannel3.setOffsetNid(30);
        deviceChannel3.setShowState(false);
        deviceChannel3.setShowVal("");
        deviceChannel3.setUpdateIdentifier("");
        deviceChannel3.getContainer().setLayout(new java.awt.BorderLayout());

        deviceField15.setIdentifier("");
        deviceField15.setLabelString("Pert. Ampl. (A):");
        deviceField15.setNumCols(4);
        deviceField15.setOffsetNid(37);
        jPanel7.add(deviceField15);

        deviceField16.setIdentifier("");
        deviceField16.setLabelString("Freq. (Hz):");
        deviceField16.setNumCols(4);
        deviceField16.setOffsetNid(38);
        jPanel7.add(deviceField16);

        deviceField17.setIdentifier("");
        deviceField17.setLabelString("Phase (Rad):");
        deviceField17.setNumCols(4);
        deviceField17.setOffsetNid(39);
        jPanel7.add(deviceField17);

        deviceField18.setIdentifier("");
        deviceField18.setLabelString("Start Time (s):");
        deviceField18.setNumCols(4);
        deviceField18.setOffsetNid(40);
        jPanel7.add(deviceField18);

        deviceField19.setIdentifier("");
        deviceField19.setLabelString("End Time (s):");
        deviceField19.setNumCols(4);
        deviceField19.setOffsetNid(41);
        jPanel7.add(deviceField19);

        deviceChannel3.getContainer().add(jPanel7, java.awt.BorderLayout.NORTH);

        deviceWave3.setIdentifier("");
        deviceWave3.setOffsetNid(31);
        deviceWave3.setUpdateExpression("");
        deviceChannel3.getContainer().add(deviceWave3, java.awt.BorderLayout.CENTER);

        jTabbedPane1.addTab("Wave 3", deviceChannel3);

        deviceChannel4.setLabelString("");
        deviceChannel4.setOffsetNid(42);
        deviceChannel4.setShowState(false);
        deviceChannel4.setShowVal("");
        deviceChannel4.setUpdateIdentifier("");
        deviceChannel4.getContainer().setLayout(new java.awt.BorderLayout());

        deviceField20.setIdentifier("");
        deviceField20.setLabelString("Pert. Ampl. (A):");
        deviceField20.setNumCols(4);
        deviceField20.setOffsetNid(49);
        jPanel8.add(deviceField20);

        deviceField21.setIdentifier("");
        deviceField21.setLabelString("Freq. (Hz):");
        deviceField21.setNumCols(4);
        deviceField21.setOffsetNid(50);
        jPanel8.add(deviceField21);

        deviceField22.setIdentifier("");
        deviceField22.setLabelString("Phase (Rad):");
        deviceField22.setNumCols(4);
        deviceField22.setOffsetNid(51);
        jPanel8.add(deviceField22);

        deviceField23.setIdentifier("");
        deviceField23.setLabelString("Start Time (s):");
        deviceField23.setNumCols(4);
        deviceField23.setOffsetNid(52);
        jPanel8.add(deviceField23);

        deviceField24.setIdentifier("");
        deviceField24.setLabelString("End Time (s):");
        deviceField24.setNumCols(4);
        deviceField24.setOffsetNid(53);
        jPanel8.add(deviceField24);

        deviceChannel4.getContainer().add(jPanel8, java.awt.BorderLayout.NORTH);

        deviceWave4.setIdentifier("");
        deviceWave4.setOffsetNid(43);
        deviceWave4.setUpdateExpression("");
        deviceChannel4.getContainer().add(deviceWave4, java.awt.BorderLayout.CENTER);

        jTabbedPane1.addTab("Wave 4", deviceChannel4);

        getContentPane().add(jTabbedPane1, java.awt.BorderLayout.CENTER);
    }// </editor-fold>//GEN-END:initComponents


    // Variables declaration - do not modify//GEN-BEGIN:variables
    private DeviceButtons deviceButtons1;
    private DeviceChannel deviceChannel1;
    private DeviceChannel deviceChannel2;
    private DeviceChannel deviceChannel3;
    private DeviceChannel deviceChannel4;
    private DeviceChoice deviceChoice1;
    private DeviceField deviceField1;
    private DeviceField deviceField10;
    private DeviceField deviceField11;
    private DeviceField deviceField12;
    private DeviceField deviceField13;
    private DeviceField deviceField14;
    private DeviceField deviceField15;
    private DeviceField deviceField16;
    private DeviceField deviceField17;
    private DeviceField deviceField18;
    private DeviceField deviceField19;
    private DeviceField deviceField2;
    private DeviceField deviceField20;
    private DeviceField deviceField21;
    private DeviceField deviceField22;
    private DeviceField deviceField23;
    private DeviceField deviceField24;
    private DeviceField deviceField3;
    private DeviceField deviceField4;
    private DeviceField deviceField5;
    private DeviceField deviceField6;
    private DeviceField deviceField7;
    private DeviceField deviceField8;
    private DeviceField deviceField9;
    private DeviceWave deviceWave1;
    private DeviceWave deviceWave2;
    private DeviceWave deviceWave3;
    private DeviceWave deviceWave4;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JPanel jPanel3;
    private javax.swing.JPanel jPanel4;
    private javax.swing.JPanel jPanel5;
    private javax.swing.JPanel jPanel6;
    private javax.swing.JPanel jPanel7;
    private javax.swing.JPanel jPanel8;
    private javax.swing.JTabbedPane jTabbedPane1;
    // End of variables declaration//GEN-END:variables

}
