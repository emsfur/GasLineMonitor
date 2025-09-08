FROM python:3.11-slim

# Set working directory
WORKDIR /app

# Copy and install dependencies
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# Copy project files
COPY . .

# Set environment variables
ENV FLASK_APP=app.py
ENV PYTHONUNBUFFERED=1 
ENV FLASK_RUN_HOST=0.0.0.0

# Expose port
EXPOSE 5000

# Run Flask app
CMD ["python", "app.py"]
