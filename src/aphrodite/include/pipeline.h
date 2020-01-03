//
// Created by n.orlov on 02/01/2020.
//

#ifndef PIPELINE_H
#define PIPELINE_H

#include <exception>
#include <iostream>

namespace aphrodite
{

class PipelineStep
{
public:
    explicit PipelineStep() = default;

    virtual void run() = 0;
    virtual ~PipelineStep() = default;
};

namespace mapper
{
    class Init : public PipelineStep
    {
    public:
        void run() override;
    };

    class Read : public PipelineStep
    {
    public:
        void run() override;
    };
    class Execute : public PipelineStep
    {
    public:
        void run() override;
    };
    class Save : public PipelineStep
    {
    public:
        void run() override;
    };
} // namespace mapper

} // namespace aphrodite

#endif // PIPELINE_H
