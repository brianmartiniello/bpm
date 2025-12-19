
import os
import plotly.graph_objects as go
import re
import subprocess
import sys

# --- CONFIGURATION ---
SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
EXE_NAME = "TestBW"
EXE_PATH = os.path.join(SCRIPT_DIR, EXE_NAME)
HTML_PATH = EXE_PATH + ".html"
# Make this significantly larger than your CPU Cache (usually 16MB-64MB)
# to ensure you are testing RAM bandwidth, not cache bandwidth.
# DATA_SIZE_B = 1024
# DATA_SIZE_B = 1 * 1024 * 1024
# DATA_SIZE_B = 2 * 1024 * 1024
DATA_SIZE_B = 4 * 1024 * 1024
# DATA_SIZE_B = 8 * 1024 * 1024
# DATA_SIZE_B = 16 * 1024 * 1024
# DATA_SIZE_B = 32 * 1024 * 1024
# DATA_SIZE_B = 64 * 1024 * 1024
# DATA_SIZE_B = 128 * 1024 * 1024
# DATA_SIZE_B = 256 * 1024 * 1024
# DATA_SIZE_B = 512 * 1024 * 1024
# DATA_SIZE_B = 1024 * 1024 * 1024
DATA_SIZE_KB = DATA_SIZE_B / 1024
DATA_SIZE_MB = DATA_SIZE_KB / 1024
DATA_SIZE_GB = DATA_SIZE_MB / 1024
MAX_THREADS = os.cpu_count()

# --- extract_elapsed_float ---
def extract_elapsed_float(text):
   # Pattern explanation:
   # ELAPSED\[  -> Matches the literal text "ELAPSED["
   # (         -> Starts a capturing group
   #  \d+      -> Matches one or more digits
   #  \.?      -> Matches an optional decimal point
   #  \d* -> Matches zero or more digits after the decimal
   # )         -> Ends the capturing group
   # \]        -> Matches the literal closing "]"
    
   pattern = r"ELAPSED\[(\d+\.?\d*)\]"
    
   match = re.search(pattern, text)
    
   if match:
      # match.group(1) pulls the content inside the parentheses
      return float(match.group(1))
    
   return None

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

   text = result.stdout.strip()
   # print(f"text ({text})")
   return extract_elapsed_float(text)

# --- main ---
def main():

   print(f"SCRIPT_DIR ({SCRIPT_DIR})")
   print(f"EXE_NAME ({EXE_NAME})")
   print(f"EXE_PATH ({EXE_PATH})")
   print(f"HTML_PATH ({HTML_PATH})")
   print(f"DATA_SIZE_B ({DATA_SIZE_B})")
   print(f"DATA_SIZE_KB ({DATA_SIZE_KB:.2f})")
   print(f"DATA_SIZE_MB ({DATA_SIZE_MB:.2f})")
   print(f"DATA_SIZE_GB ({DATA_SIZE_GB:.2f})")
   print(f"{'Threads':<8} | {'Mem time (s)':<12} | {'Mem GB/s':<12} | {'Comp time (s)':<14} | {'Comp Speedup':<12}")
   print("-" * 72)

   # --- DATA COLLECTION ---
   threads, mem_bw, comp_scale = [], [], []
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

      print(f"{t:<8} | {m_time:<12.6f} | {bw:<12.2f} | {c_time:<14.6f} | {speedup:<12.2f}")

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

   fig.write_html(HTML_PATH)
   # fig.show()

if __name__ == "__main__":
   main()
