
import os
import plotly.graph_objects as go
import subprocess
import sys

# --- CONFIGURATION ---
SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
EXE_PATH = os.path.join(SCRIPT_DIR, "TestBW")
# ~800MB. 
# Make this significantly larger than your CPU Cache (usually 16MB-64MB)
# to ensure you are testing RAM bandwidth, not cache bandwidth.
DATA_SIZE_B = 800 * 1024 * 1024
DATA_SIZE_GB = DATA_SIZE_B / 1e9
MAX_THREADS = os.cpu_count()

# --- resource_path ---
def resource_path(relative_path):
   """ Get absolute path to resource, works for dev and for PyInstaller """
   try:
      # PyInstaller creates a temp folder and stores path in _MEIPASS
      base_path = sys._MEIPASS
   except Exception:
      base_path = os.path.abspath(".")

   return os.path.join(base_path, relative_path)

# --- run_bench ---
def run_bench(num_threads, mode):
   # Arguments: <threads> <bytes> <mode>
   args = [EXE_PATH, str(num_threads), str(DATA_SIZE_B), mode]
   result = subprocess.run(args, capture_output=True, text=True)
    
   if result.returncode != 0:
      print(f"Error {EXE_PATH}: {result.stderr}")
      return 0.0

   return float(result.stdout.strip())

# --- main ---
def main():
   # --- DATA COLLECTION ---
   threads, mem_bw, comp_scale = [], [], []

   print(f"SCRIPT_DIR ({SCRIPT_DIR})")
   print(f"EXE_PATH ({EXE_PATH})")
   print(f"DATA_SIZE_B ({DATA_SIZE_B})")
   print(f"DATA_SIZE_GB ({DATA_SIZE_GB:.2f})")
   print(f"{'Threads':<8} | {'Mem GB/s':<12} | {'Comp Speedup':<12}")
   print("-" * 40)

   for t in range(1, MAX_THREADS + 1):
      # Take the minimum time (best performance) across trials
      m_time = min([run_bench(t, "memory") for _ in range(3)])
      c_time = min([run_bench(t, "compute") for _ in range(3)])
       
      if t == 1: 
         base_c_time = c_time
           
      bw = DATA_SIZE_GB / m_time
      speedup = base_c_time / c_time
       
      threads.append(t)
      mem_bw.append(bw)
      comp_scale.append(speedup)
      print(f"{t:<8} | {bw:<12.2f} | {speedup:<12.2f}")

   # --- PLOTTING ---
   fig = go.Figure()

   # Memory Bandwidth Trace
   fig.add_trace(
      go.Scatter(
         x=threads,
         y=mem_bw,
         name='Mem Bandwidth (GB/s)',
         line=dict(color='blue',
                   width=3),
         yaxis="y1"
      )
   )

   # Compute Scaling Trace
   fig.add_trace(
      go.Scatter(
         x=threads,
         y=comp_scale,
         name='Compute Speedup (x)',
         line=dict(color='red',
                   width=3,
                   dash='dash'),
         yaxis="y2"
      )
   )

   fig.update_layout(
       title=f'Performance Scaling ({DATA_SIZE_GB:.2f} GB Data)',
       xaxis=dict(title='Number of Threads',
                  tickmode='linear',
                  dtick=1),
       yaxis=dict(title='Bandwidth (GB/s)',
                  side='left'),
       yaxis2=dict(title='Compute Speedup Factor',
                   overlaying='y',
                   side='right',
                   range=[0, MAX_THREADS]),
       legend=dict(x=0.01,
                   y=0.99),
       template='plotly_white',
       hovermode='x unified'
   )

   fig.show()

if __name__ == "__main__":
   main()
