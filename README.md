# big-dripper
Water curtain display controller and MCU code

# Organization
- `/big_dripper/` - All application code for linux rpi application lives  (probably python application)
- `requirements.txt` - All python requirements should be tracked here
- `/share/` - All other configurations files for linux live here (ex. init.d scripts /boot/config)
- `/dripperator/` - PlatformIO project for pump IO board control

## Setup For Python App Development
clone this git repository and then run (make sure you have pip and venv installed):

``` bash
python -m venv venv
source venv/bin/activate
pip install -r linux/python/requirements.txt
```

Then point your development enviroment to venv.