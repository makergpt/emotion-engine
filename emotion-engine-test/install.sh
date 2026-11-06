#!/bin/bash
set -e

echo "[INFO] Building and installing Emotion Engine CLI..."

# 1. Build project
if [ -f "./build.sh" ]; then
    ./build.sh
else
    echo "[ERROR] build.sh not found."
    exit 1
fi

ENGINE_DIR="$(pwd)"

# 2. Create the enhanced CLI wrapper
cat << WRAPPER > emotion-cli-wrapper
#!/bin/bash
cd "$ENGINE_DIR" || { echo "[ERROR] Engine directory not found at $ENGINE_DIR"; exit 1; }

CMD=\$1
shift

case "\$CMD" in
    "start"|"run")
        echo "[INFO] Starting Emotion Engine workstation..."
        ./build.sh && ./emotion-engine
        ;;
    "launch")
        TARGET_APP=\$1
        if [ -z "\$TARGET_APP" ]; then
            echo "[ERROR] Please specify an app name. Usage: emotion launch <app_name>"
            exit 1
        fi
        echo "[INFO] Intercepting and launching [\$TARGET_APP] via Emotion EUTL..."
        ./build.sh && ./emotion-engine "\$TARGET_APP"
        ;;
    "test")
        echo "[INFO] Running Emotion Engine diagnostic test suite..."
        ./build.sh && ./emotion-engine "DiagnosticTestMode"
        ;;
    "shutdown")
        echo "[INFO] Sending shutdown signal to active Emotion Engine processes..."
        pkill -f emotion-engine || echo "[INFO] No active engine processes found."
        echo "[SUCCESS] Emotion Engine shutdown complete."
        ;;
    *)
        echo "Emotion Engine CLI Master"
        echo "Usage:"
        echo "  emotion start             - Launch core workstation"
        echo "  emotion launch <app>      - Intercept and run external app (e.g. sober)"
        echo "  emotion test              - Run engine diagnostic test suite"
        echo "  emotion shutdown          - Cleanly terminate active engine sessions"
        ;;
esac
WRAPPER

# 3. Register globally
echo "[INFO] Registering 'emotion' command in /usr/local/bin..."
sudo mv emotion-cli-wrapper /usr/local/bin/emotion
sudo chmod +x /usr/local/bin/emotion

echo "[SUCCESS] Installation complete! Try running 'emotion start' or 'emotion launch sober'."
