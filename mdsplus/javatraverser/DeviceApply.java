public class DeviceApply extends DeviceControl
{
    public DeviceApply()
    {
        setText("Apply");
    }
    void setReadOnly(boolean readOnly)
    {
           setEnabled(!readOnly);
    }
    protected void doOperation(DeviceSetup deviceSetup)
    {
        deviceSetup.apply();
        check();
    }

}
