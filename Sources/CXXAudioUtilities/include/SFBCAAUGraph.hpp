//
// Copyright (c) 2021 - 2024 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/SFBAudioUtilities
// MIT license
//

#pragma once

#import <map>
#import <vector>

#import <AudioToolbox/AUGraph.h>

#import "SFBCAException.hpp"
#import "SFBCAStreamBasicDescription.hpp"

CF_ASSUME_NONNULL_BEGIN

namespace SFB {

/// A wrapper around @c AUGraph
class CAAUGraph
{

public:
	/// Creates a @c CAAUGraph
	CAAUGraph() noexcept = default;

	// This class is non-copyable
	CAAUGraph(const CAAUGraph& rhs) = delete;

	// This class is non-assignable
	CAAUGraph& operator=(const CAAUGraph& rhs) = delete;

	/// Destroys the @c CAAUGraph and releases all associated resources.
	inline ~CAAUGraph()
	{
		if(mAUGraph)
			DisposeAUGraph(mAUGraph);
	}

	/// Move constructor
	CAAUGraph(CAAUGraph&& rhs) noexcept
	: mAUGraph{rhs.mAUGraph}
	{
		rhs.mAUGraph = nullptr;
	}

	/// Move assignment operator
	CAAUGraph& operator=(CAAUGraph&& rhs) noexcept
	{
		if(this != &rhs) {
			mAUGraph = rhs.mAUGraph;
			rhs.mAUGraph = nullptr;
		}
		return *this;
	}

	/// Returns @c true if this object's internal @c AUGraph is not @c nullptr
	inline explicit operator bool() const noexcept
	{
		return mAUGraph != nullptr;
	}

	/// Returns @c true if this object's internal @c AUGraph is @c nullptr
	inline bool operator!() const noexcept
	{
		return !operator bool();
	}

	/// Returns @c true if this object's internal @c AUGraph is not @c nullptr
	inline bool IsValid() const noexcept
	{
		return operator bool();
	}

	/// Returns the object's internal @c AUGraph
	inline operator AUGraph const _Nullable () const noexcept
	{
		return mAUGraph;
	}

	/// Creates a new audio processing graph.
	/// @throw @c std::system_error
	void New()
	{
		Dispose();
		auto result = NewAUGraph(&mAUGraph);
		ThrowIfCAAUGraphError(result, "NewAUGraph");
	}

	/// Disposes an existing audio processing graph.
	/// @throw @c std::system_error
	void Dispose()
	{
		if(mAUGraph) {
			auto result = DisposeAUGraph(mAUGraph);
			ThrowIfCAAUGraphError(result, "DisposeAUGraph");
			mAUGraph = nullptr;
		}
	}

	// MARK: - Node State

	/// Adds a node to the audio processing graph.
	/// @throw @c std::system_error
	AUNode AddNode(const AudioComponentDescription *inDescription)
	{
		AUNode node = -1;
		auto result = AUGraphAddNode(mAUGraph, inDescription, &node);
		ThrowIfCAAUGraphError(result, "AUGraphAddNode");
		return node;
	}

	/// Removes a node from the audio processing graph.
	/// @throw @c std::system_error
	void RemoveNode(AUNode inNode)
	{
		auto result = AUGraphRemoveNode(mAUGraph, inNode);
		ThrowIfCAAUGraphError(result, "AUGraphRemoveNode");
	}

	/// Returns the number of nodes in the audio processing graph.
	/// @throw @c std::system_error
	UInt32 GetNodeCount() const
	{
		UInt32 numberOfNodes = 0;
		auto result = AUGraphGetNodeCount(mAUGraph, &numberOfNodes);
		ThrowIfCAAUGraphError(result, "AUGraphGetNodeCount");
		return numberOfNodes;
	}

	/// Returns the node at a given index
	/// @throw @c std::system_error
	AUNode GetIndNode(UInt32 inIndex) const
	{
		AUNode node = -1;
		auto result = AUGraphGetIndNode(mAUGraph, inIndex, &node);
		ThrowIfCAAUGraphError(result, "AUGraphGetIndNode");
		return node;
	}

	/// Returns information about a particular AUNode.
	/// @throw @c std::system_error
	void NodeInfo(AUNode inNode, AudioComponentDescription * __nullable outDescription, AudioUnit __nullable * __nullable outAudioUnit) const
	{
		auto result = AUGraphNodeInfo(mAUGraph, inNode, outDescription, outAudioUnit);
		ThrowIfCAAUGraphError(result, "AUGraphNodeInfo");
	}


#if !TARGET_OS_IPHONE
	// MARK: - Sub Graphs

	/// Creates a node that will represent a sub graph.
	/// @throw @c std::system_error
	AUNode NewNodeSubGraph()
	{
		AUNode node = -1;
		auto result = AUGraphNewNodeSubGraph(mAUGraph, &node);
		ThrowIfCAAUGraphError(result, "AUGraphNewNodeSubGraph");
		return node;
	}

	/// Returns the sub graph represented by a particular AUNode.
	/// @throw @c std::system_error
	AUGraph GetNodeInfoSubGraph(AUNode inNode) const
	{
		AUGraph subGraph = nullptr;
		auto result = AUGraphGetNodeInfoSubGraph(mAUGraph, inNode, &subGraph);
		ThrowIfCAAUGraphError(result, "AUGraphGetNodeInfoSubGraph");
		return subGraph;
	}

	/// Returns @c true if the node represents a sub graph.
	/// @throw @c std::system_error
	bool IsNodeSubGraph(AUNode inNode) const
	{
		Boolean flag = 0;
		auto result = AUGraphIsNodeSubGraph(mAUGraph, inNode, &flag);
		ThrowIfCAAUGraphError(result, "AUGraphIsNodeSubGraph");
		return flag != 0;
	}
#endif

	// MARK: - Node Interactions

	/// Connects a node's output to a node's input.
	/// @throw @c std::system_error
	void ConnectNodeInput(AUNode inSourceNode, UInt32 inSourceOutputNumber, AUNode inDestNode, UInt32 inDestInputNumber)
	{
		auto result = AUGraphConnectNodeInput(mAUGraph, inSourceNode, inSourceOutputNumber, inDestNode, inDestInputNumber);
		ThrowIfCAAUGraphError(result, "AUGraphConnectNodeInput");
	}

	/// Sets a callback for the specified node's specified input.
	/// @throw @c std::system_error
	void SetNodeInputCallback(AUNode inDestNode, UInt32 inDestInputNumber, const AURenderCallbackStruct *inInputCallback)
	{
		auto result = AUGraphSetNodeInputCallback(mAUGraph, inDestNode, inDestInputNumber, inInputCallback);
		ThrowIfCAAUGraphError(result, "AUGraphSetNodeInputCallback");
	}

	/// Disconnects a node's input.
	/// @throw @c std::system_error
	void DisconnectNodeInput(AUNode inDestNode, UInt32 inDestInputNumber)
	{
		auto result = AUGraphDisconnectNodeInput(mAUGraph, inDestNode, inDestInputNumber);
		ThrowIfCAAUGraphError(result, "AUGraphDisconnectNodeInput");
	}

	/// Clears all of the interactions in a graph.
	/// @throw @c std::system_error
	void ClearConnections()
	{
		auto result = AUGraphClearConnections(mAUGraph);
		ThrowIfCAAUGraphError(result, "AUGraphClearConnections");
	}

	/// Returns the number of interactions in the audio processing graph.
	/// @throw @c std::system_error
	UInt32 GetNumberOfInteractions() const
	{
		UInt32 numberOfInteractions = 0;
		auto result = AUGraphGetNumberOfInteractions(mAUGraph, &numberOfInteractions);
		ThrowIfCAAUGraphError(result, "AUGraphGetNumberOfInteractions");
		return numberOfInteractions;
	}

	/// Returns information about a particular interaction in a graph.
	/// @throw @c std::system_error
	AUNodeInteraction GetInteractionInfo(UInt32 inInteractionIndex) const
	{
		AUNodeInteraction interaction{};
		auto result = AUGraphGetInteractionInfo(mAUGraph, inInteractionIndex, &interaction);
		ThrowIfCAAUGraphError(result, "AUGraphGetInteractionInfo");
		return interaction;
	}

	/// Returns the number of interactions of a graph's node.
	/// @throw @c std::system_error
	UInt32 CountNodeInteractions(AUNode inNode) const
	{
		UInt32 numberOfInteractions = 0;
		auto result = AUGraphCountNodeInteractions(mAUGraph, inNode, &numberOfInteractions);
		ThrowIfCAAUGraphError(result, "AUGraphCountNodeInteractions");
		return numberOfInteractions;
	}

	/// Retrieves information about the interactions in a graph for a given node.
	/// @throw @c std::system_error
	void GetNodeInteractions(AUNode inNode, UInt32 *ioNumInteractions, AUNodeInteraction *outInteractions) const
	{
		auto result = AUGraphGetNodeInteractions(mAUGraph, inNode, ioNumInteractions, outInteractions);
		ThrowIfCAAUGraphError(result, "AUGraphGetNodeInteractions");
	}

	// MARK: -

	/// Updates the state of a running AUGraph.
	/// @throw @c std::system_error
	bool Update()
	{
		Boolean flag = 0;
		auto result = AUGraphUpdate(mAUGraph, &flag);
		ThrowIfCAAUGraphError(result, "AUGraphUpdate");
		return flag != 0;
	}

	// MARK: - State Management

	/// Opens a graph.
	/// @throw @c std::system_error
	void Open()
	{
		auto result = AUGraphOpen(mAUGraph);
		ThrowIfCAAUGraphError(result, "AUGraphOpen");
	}

	/// Closes a graph.
	/// @throw @c std::system_error
	void Close()
	{
		auto result = AUGraphClose(mAUGraph);
		ThrowIfCAAUGraphError(result, "AUGraphClose");
	}

	/// Initializes a graph.
	/// @throw @c std::system_error
	void Initialize()
	{
		auto result = AUGraphInitialize(mAUGraph);
		ThrowIfCAAUGraphError(result, "AUGraphInitialize");
	}

	/// Uninitialize a graph.
	/// @throw @c std::system_error
	void Uninitialize()
	{
		auto result = AUGraphUninitialize(mAUGraph);
		ThrowIfCAAUGraphError(result, "AUGraphUninitialize");
	}

	/// Starts a graph.
	/// @throw @c std::system_error
	void Start()
	{
		auto result = AUGraphStart(mAUGraph);
		ThrowIfCAAUGraphError(result, "AUGraphStart");
	}

	/// Stops a graph.
	/// @throw @c std::system_error
	void Stop()
	{
		auto result = AUGraphStop(mAUGraph);
		ThrowIfCAAUGraphError(result, "AUGraphStop");
	}

	/// Returns @c true if the audio processing graph is open.
	/// @throw @c std::system_error
	bool IsOpen() const
	{
		Boolean flag = 0;
		auto result = AUGraphIsOpen(mAUGraph, &flag);
		ThrowIfCAAUGraphError(result, "AUGraphIsOpen");
		return flag != 0;
	}

	/// Returns @c true if the audio processing graph is initialized.
	/// @throw @c std::system_error
	bool IsInitialized() const
	{
		Boolean flag = 0;
		auto result = AUGraphIsInitialized(mAUGraph, &flag);
		ThrowIfCAAUGraphError(result, "AUGraphIsInitialized");
		return flag != 0;
	}

	/// Returns @c true if the audio processing graph is running.
	/// @throw @c std::system_error
	bool IsRunning() const
	{
		Boolean flag = 0;
		auto result = AUGraphIsRunning(mAUGraph, &flag);
		ThrowIfCAAUGraphError(result, "AUGraphIsRunning");
		return flag != 0;
	}

	// MARK: - Utilities

	/// Returns a short-term running average of the current CPU load of the graph.
	/// @throw @c std::system_error
	Float32 GetCPULoad() const
	{
		Float32 value = 0;
		auto result = AUGraphGetCPULoad(mAUGraph, &value);
		ThrowIfCAAUGraphError(result, "AUGraphGetCPULoad");
		return value;
	}

	/// Returns the max CPU load of the graph since this call was last made or the graph was last started.
	/// @throw @c std::system_error
	Float32 GetMaxCPULoad() const
	{
		Float32 value = 0;
		auto result = AUGraphGetMaxCPULoad(mAUGraph, &value);
		ThrowIfCAAUGraphError(result, "AUGraphGetMaxCPULoad");
		return value;
	}

	/// Adds a notification callback.
	/// @throw @c std::system_error
	void AddRenderNotify(AURenderCallback inCallback, void * __nullable inRefCon)
	{
		auto result = AUGraphAddRenderNotify(mAUGraph, inCallback, inRefCon);
		ThrowIfCAAUGraphError(result, "AUGraphAddRenderNotify");
	}

	/// Removes a notification callback.
	/// @throw @c std::system_error
	void RemoveRenderNotify(AURenderCallback inCallback, void * __nullable inRefCon)
	{
		auto result = AUGraphRemoveRenderNotify(mAUGraph, inCallback, inRefCon);
		ThrowIfCAAUGraphError(result, "AUGraphRemoveRenderNotify");
	}

	// MARK: - Helpers

	/// Returns the graph's nodes.
	/// @throw @c std::system_error
	std::vector<AUNode> Nodes() const
	{
		auto nodeCount = GetNodeCount();
		auto nodes = std::vector<AUNode>(nodeCount);
		for(UInt32 i = 0; i < nodeCount; ++i) {
			auto node = GetIndNode(i);
			nodes.push_back(node);
		}
		return nodes;
	}

	/// Returns a node's interactions.
	/// @throw @c std::system_error
	std::vector<AUNodeInteraction> NodeInteractions(AUNode inNode) const
	{
		auto interactionCount = CountNodeInteractions(inNode);
		auto interactions = std::vector<AUNodeInteraction>(interactionCount);
		GetNodeInteractions(inNode, &interactionCount, &interactions[0]);
		return interactions;
	}

	/// Returns the graph's nodes and their interactions.
	/// @throw @c std::system_error
	std::map<AUNode, std::vector<AUNodeInteraction>> NodesAndInteractions() const
	{
		auto nodes = Nodes();
		auto nodesAndInteractions = std::map<AUNode, std::vector<AUNodeInteraction>>();
		for(auto node : nodes)
			nodesAndInteractions[node] = NodeInteractions(node);
		return nodesAndInteractions;
	}

	/// Returns the audio processing graph's latency.
	/// @throw @c std::system_error
	Float64 Latency() const
	{
		Float64 latency = 0;
		auto nodeCount = GetNodeCount();
		for(UInt32 i = 0; i < nodeCount; ++i) {
			auto node = GetIndNode(i);
			AudioUnit au = nullptr;
			NodeInfo(node, nullptr, &au);

			Float64 auLatency = 0;
			UInt32 dataSize = sizeof(auLatency);
			auto result = AudioUnitGetProperty(au, kAudioUnitProperty_Latency, kAudioUnitScope_Global, 0, &auLatency, &dataSize);
			ThrowIfCAAudioUnitError(result, "AudioUnitGetProperty (kAudioUnitProperty_Latency, kAudioUnitScope_Global)");

			latency += auLatency;
		}

		return latency;
	}

	/// Returns the audio processing graph's tail time.
	/// @throw @c std::system_error
	Float64 TailTime() const
	{
		Float64 tailTime = 0;
		auto nodeCount = GetNodeCount();
		for(UInt32 i = 0; i < nodeCount; ++i) {
			auto node = GetIndNode(i);
			AudioUnit au = nullptr;
			NodeInfo(node, nullptr, &au);

			Float64 auTailTime = 0;
			UInt32 dataSize = sizeof(auTailTime);
			auto result = AudioUnitGetProperty(au, kAudioUnitProperty_TailTime, kAudioUnitScope_Global, 0, &auTailTime, &dataSize);
			ThrowIfCAAudioUnitError(result, "AudioUnitGetProperty (kAudioUnitProperty_TailTime, kAudioUnitScope_Global)");

			tailTime += auTailTime;
		}

		return tailTime;
	}

private:

	/// The underlying @c AUGraph object
	AUGraph _Nullable mAUGraph = nullptr;

};

} // namespace SFB

CF_ASSUME_NONNULL_END
