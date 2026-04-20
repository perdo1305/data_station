FROM osrf/ros:jazzy-desktop

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV ROS_DISTRO=jazzy
ENV PYTHONUNBUFFERED=1

# Install system dependencies
RUN apt-get update && apt-get install -y \
    python3-pip \
    python3-can \
    can-utils \
    git \
    build-essential \
    vim \
    wget \
    curl \
    iputils-ping \
    net-tools \
    && rm -rf /var/lib/apt/lists/*

# Create workspace
WORKDIR /root/lart_ws

# Copy the ROS packages and dashboard
COPY src ./src

# Install ROS 2 extensions
RUN apt-get update && \
    apt-get install -y \
    ros-jazzy-desktop \
    python3-colcon-common-extensions \
    && rm -rf /var/lib/apt/lists/*

# Install Python dependencies
RUN pip install --break-system-packages \
    python-can>=4.0 \
    gpiod>=2.0 \
    adafruit-circuitpython-neopixel>=6.0 \
    adafruit-blinka>=8.0 \
    pygame>=2.5 \
    2>&1 || echo "Some packages may not be available on this architecture"

# Build the workspace
RUN . /opt/ros/jazzy/setup.sh && \
    colcon build --symlink-install

# Setup entrypoint script
RUN echo "#!/bin/bash" > /root/entrypoint.sh && \
    echo "source /opt/ros/jazzy/setup.bash" >> /root/entrypoint.sh && \
    echo "source /root/lart_ws/install/setup.bash" >> /root/entrypoint.sh && \
    echo 'exec "$@"' >> /root/entrypoint.sh && \
    chmod +x /root/entrypoint.sh

RUN echo "source /opt/ros/jazzy/setup.bash" >> /root/.bashrc && \
    echo "source /root/lart_ws/install/setup.bash" >> /root/.bashrc

# Set the entry point
ENTRYPOINT ["/root/entrypoint.sh"]
CMD ["/bin/bash", "-i"]
