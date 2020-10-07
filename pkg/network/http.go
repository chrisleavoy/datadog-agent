package network

// HTTPTracker tracks HTTP requests/responses
type HTTPTracker interface {
	GetHTTPStats() map[httpKey]httpInfo
	GetStats() map[string]int64
	Close()
}

type nullHTTPTracker struct{}

// NewNullHTTPTracker returns a dummy implementation of HttpTracker
func NewNullHTTPTracker() HTTPTracker {
	return nullHTTPTracker{}
}

func (nullHTTPTracker) GetHTTPStats() map[httpKey]httpInfo {
	return nil
}

func (nullHTTPTracker) GetStats() map[string]int64 {
	return map[string]int64{
		"socket_polls":         0,
		"packets_processed":    0,
		"packets_captured":     0,
		"packets_dropped":      0,
		"packets_skipped":      0,
		"packets_valid":        0,
		"http_requests":        0,
		"http_responses":       0,
		"connections_flushed":  0,
		"connections_closed":   0,
		"timestamp_micro_secs": 0,
	}
}

func (nullHTTPTracker) Close() {}

var _ HTTPTracker = nullHTTPTracker{}
