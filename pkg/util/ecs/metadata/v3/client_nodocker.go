// Unless explicitly stated otherwise all files in this repository are licensed
// under the Apache License Version 2.0.
// This product includes software developed at Datadog (https://www.datadoghq.com/).
// Copyright 2019 Datadog, Inc

// +build !docker

package v3

import "github.com/DataDog/datadog-agent/pkg/util/docker"

type Client struct{}

func NewClientForCurrentTask() (*Client, error) {
	return nil, docker.ErrDockerNotCompiled
}

func NewClientForContainer(containerID string) (*Client, error) {
	return nil, docker.ErrDockerNotCompiled
}
