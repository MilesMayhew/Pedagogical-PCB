import serial
import numpy as np
import wave
import time
import simpleaudio as sa  # For playback

# === USER CONFIG ===
PORT = "COM3"
BAUD_RATE = 460800
SAMPLE_RATE = 44100
DURATION = 5
SAMPLE_WIDTH = 1  # bytes (8-bit)
CHUNK_SIZE = 2048
FILENAME = "mic_record.wav"
# ====================

num_samples = int(SAMPLE_RATE * DURATION)
bytes_to_read_total = num_samples * SAMPLE_WIDTH

print(f"🎙️ Recording configuration:")
print(f"  Sample rate: {SAMPLE_RATE} Hz")
print(f"  Duration:    {DURATION} s")
print(f"  Expected samples: {num_samples}")
print(f"  Expected bytes:   {bytes_to_read_total}")
print(f"  UART baud:   {BAUD_RATE} bps\n")

# === READ UART DATA ===
ser = serial.Serial(PORT, BAUD_RATE, timeout=1)
print(f"🔌 Reading from {PORT} ...")

all_bytes = bytearray()
start_time = time.time()
while len(all_bytes) < bytes_to_read_total:
    data = ser.read(CHUNK_SIZE)
    if data:
        all_bytes.extend(data)
ser.close()
elapsed = time.time() - start_time

print(f"📦 Expected {num_samples} samples ({bytes_to_read_total} bytes)")
print(f"✅ Received {len(all_bytes)} bytes in {elapsed:.2f} s")

# === CONVERT TO NUMPY ===
raw_values = np.frombuffer(all_bytes, dtype=np.uint8)
samples_centered = raw_values.astype(np.float32) - 128.0  # center around 0

# === SAVE AS WAV ===
with wave.open(FILENAME, 'wb') as wf:
    wf.setnchannels(1)
    wf.setsampwidth(SAMPLE_WIDTH)
    wf.setframerate(SAMPLE_RATE)
    wf.writeframes(raw_values.tobytes())
print(f"💾 Saved audio to: {FILENAME}\n")

# === AUDIO PLAYBACK ===
print("▶️ Playing back the recorded audio...")
play_obj = sa.play_buffer(raw_values, 1, SAMPLE_WIDTH, SAMPLE_RATE)
play_obj.wait_done()
print("✅ Playback finished.")
